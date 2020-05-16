#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>

#include "inputreader.h"
#include "place_queue.h"
#include "../common/utils.h"

#define MAX_THREAD 100

int pl = 1;
place_queue pq;
pthread_mutex_t pl_mut = PTHREAD_MUTEX_INITIALIZER;
sem_t nthreads;
sem_t nplaces;
int limit_threads = 0;
int limit_places = 0;

void* handleRequest(void* arg) {
    pthread_detach(pthread_self());

    struct fifo_msg c_msg = *(struct fifo_msg *) arg;
    free(arg);

    // Reports that message was recieved
    printLog(c_msg.i, c_msg.pid, c_msg.tid, c_msg.dur, c_msg.pl, RECEIVED);

    // Opens private fifo for sending response, waiting for client to open other side
    char fifoname[MAX_FIFONAME];
    sprintf(fifoname, "/tmp/%d.%ld", c_msg.pid, c_msg.tid);
    int priv_fd;
    if ((priv_fd = open(fifoname, O_WRONLY)) < 0) {
        // If an error occurs, report that client has given up
        fprintf(stderr, "Cannot open client %d FIFO for WRITING!", c_msg.i);
        printLog(c_msg.i, getpid(), pthread_self(), c_msg.dur, -1, GAVEUP);

        if (limit_threads) { sem_post(&nthreads); }
        return NULL;
    }

    // Assign a place to the request
    
    int place;
    if (limit_places) {
        sem_wait(&nplaces);
        pthread_mutex_lock(&pl_mut);
        place = usePlace(&pq);
        pthread_mutex_unlock(&pl_mut);
    } else {
        pthread_mutex_lock(&pl_mut);
        place = pl;
        pl++;
        pthread_mutex_unlock(&pl_mut);
    }
    

    // Send client a response through private fifo
    struct fifo_msg response = {c_msg.i, getpid(), pthread_self(), c_msg.dur, place};
    if (write(priv_fd, &response, sizeof(response)) < 0) {
        // If an error occurs, report that client has given up
        fprintf(stderr, "Cannot write to client %d FIFO!", c_msg.i);
        printLog(c_msg.i, getpid(), pthread_self(), c_msg.dur, -1, GAVEUP);
        close(priv_fd);

        if (limit_threads) { sem_post(&nthreads); }
        if (limit_places) { 
            pthread_mutex_lock(&pl_mut);
            makePlaceAvailable(&pq, place);
            pthread_mutex_unlock(&pl_mut);
            sem_post(&nplaces); 
        }
        return NULL;
    }
    
    close(priv_fd); // All private fifo communication done

    // Report that client has entered bathroom
    printLog(c_msg.i, getpid(), pthread_self(), c_msg.dur, place, ENTER);

    usleep(c_msg.dur * 1000); // Waits for duration of request

    // Report that client has finished using bathroom
    printLog(c_msg.i, getpid(), pthread_self(), c_msg.dur, place, TIMEUP);

    if (limit_threads) { sem_post(&nthreads); }
    if (limit_places) { 
        pthread_mutex_lock(&pl_mut);
        makePlaceAvailable(&pq, place);
        pthread_mutex_unlock(&pl_mut);
        sem_post(&nplaces); 
    }
    return NULL;
}

void* sendTooLate(void* arg) {
    pthread_detach(pthread_self());

    struct fifo_msg c_msg = *(struct fifo_msg *) arg;
    free(arg);

    // Reports that message was recieved
    printLog(c_msg.i, c_msg.pid, c_msg.tid, c_msg.dur, c_msg.pl, RECEIVED);

    // Opens private fifo for sending response, waiting for client to open other side
    char fifoname[MAX_FIFONAME];
    sprintf(fifoname, "/tmp/%d.%ld", c_msg.pid, c_msg.tid);
    int priv_fd;
    if ((priv_fd = open(fifoname, O_WRONLY)) < 0) {
        fprintf(stderr, "Cannot open %s for WRITING!\n", fifoname);
        printLog(c_msg.i, getpid(), pthread_self(), c_msg.dur, -1, GAVEUP);
        
        if (limit_threads) { sem_post(&nthreads); }
        return NULL;
    }

    // Send client a response through private fifo
    struct fifo_msg response = {c_msg.i, getpid(), pthread_self(), -1, -1};
    if (write(priv_fd, &response, sizeof(response)) < 0) {
        // If an error occurs, report that client has given up
        fprintf(stderr, "Cannot write to client %d FIFO!", c_msg.i);
        printLog(c_msg.i, getpid(), pthread_self(), c_msg.dur, -1, GAVEUP);
        close(priv_fd);
        
        if (limit_threads) { sem_post(&nthreads); }
        return NULL;
    }

    close(priv_fd); // All private fifo communication done

    // Report that client was warned bathroom was closed
    printLog(c_msg.i, getpid(), pthread_self(), -1, -1, TOOLATE);

    if (limit_threads) { sem_post(&nthreads); }
    return NULL;
}

int main(int argc, char* argv[]) {
    setStart();

    struct Qarg args = processArgs(argc,argv); // Process console arguments
    if (args.nthreads != 0) { limit_threads = 1; }
    if (args.nplaces != 0) { limit_places = 1; }
    
    // Create server public fifo
    if (mkfifo(args.fifoname, 0660) == -1) {
        perror("Error creating Server FIFO");
        if (errno != EEXIST) { // If the FIFO exists, it will try to open it anyways, otherwise, exit
            exit(2);
        }
    }

    // Open public fifo for reading
    int fifo_fd;
    if ((fifo_fd = open(args.fifoname, O_RDONLY)) < 0) {
        perror("Cannot open Server FIFO for reading!");
        if (unlink(args.fifoname) < 0) {
            perror("Cannot delete FIFO");
        }
        exit(3);
    }

    // Request-handling loop, stops after runtime has passed
    if (limit_threads) {
        sem_init(&nthreads, 0, args.nthreads);
    }
    if (limit_places) {
        sem_init(&nplaces, 0, args.nplaces);
        pq = createPlaceQueue(args.nplaces);
        fillPlaceQueue(&pq);
    }
    struct fifo_msg msg;
    while (elapsedTime() < args.numberSeconds) {
        // Reads a message from public fifo and if it is a valid one, processes it
        if (read(fifo_fd, &msg, sizeof(msg)) > 0) {
            // Creates a copy of read message for passing to thread
            struct fifo_msg* cpy = malloc(sizeof(struct fifo_msg));
            *cpy = msg;
            // Creates thread for processing request
            if (limit_threads) { sem_wait(&nthreads); }
            pthread_t t;
            if (pthread_create(&t, NULL, handleRequest, cpy) != 0) {
                perror("Thread creation failed");
                free(cpy);
            }
        }
    }

    if (unlink(args.fifoname) < 0) {
        perror("Cannot delete FIFO");
    }

    // If after runtime has passed there is a message in the fifo, indicate that it is too late
    // to use the bathroom
    while (read(fifo_fd, &msg, sizeof(msg)) > 0) {
        struct fifo_msg* cpy = malloc(sizeof(struct fifo_msg));
        *cpy = msg;
        if (limit_threads) { sem_wait(&nthreads); }
        pthread_t t;
        if (pthread_create(&t, NULL, sendTooLate, cpy) != 0) {
            perror("Thread creation failed");
            free(cpy);
        }
    }

    // Delete public fifo
    close(fifo_fd);
    
    pthread_exit(0);
}
