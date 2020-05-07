#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

#include "inputreader.h"
#include "../common/utils.h"

struct client_msg {
    char* fifoname;
    int i;
};

int closed = 0;

void* requestSender(void* arg) {
    pthread_detach(pthread_self());

    struct client_msg c_msg = *(struct client_msg*) arg; // Converts argument into message struct

    int dur = rand() % 800 + 200; // Generates random duration bewtween 200 and 999 ms

    // Opens server fifo for writing messafe
    int fifo_fd;
    if ((fifo_fd = open(c_msg.fifoname, O_WRONLY | O_NONBLOCK)) < 0) {
        // If an error occurs report a failure and assume the server is closed
        perror("Cannot open server FIFO for WRITING");
        printLog(c_msg.i, getpid(), pthread_self(), dur, -1, FAILED);
        closed = 1;
        return NULL;
    }

    // Creates private fifo, done before sending message to avoid access before it is created
    char fifoname[MAX_FIFONAME];
    sprintf(fifoname, "/tmp/%d.%ld", getpid(), pthread_self());
    if (mkfifo(fifoname, 0660) == -1) {
        // If an error occurs report a failure
        perror("Error creating private FIFO");
        printLog(c_msg.i, getpid(), pthread_self(), dur, -1, FAILED);
        return NULL;
    }

    // Sends request message to server public fifo
    char msg[MAX_MSG];
    sprintf(msg, "[ %d, %d, %ld, %d, %d ]\n", c_msg.i, getpid(), pthread_self(), dur, -1);
    if (write(fifo_fd, &msg, strlen(msg)) < 0) {
        // If an error ocurrs report a failure, delete the private fifo and assume the server is closed
        printLog(c_msg.i, getpid(), pthread_self(), dur, -1, FAILED);
        if (unlink(fifoname) < 0) {
            perror("Cannot delete private FIFO");
        }
        closed = 1;
        return NULL;
    } else {
        printLog(c_msg.i, getpid(), pthread_self(), dur, -1, IWANT);
    }

    close(fifo_fd); // All public fifo communication done

    // Open private fifo for reading, waiting for server to open other side
    int priv_fd;
    if ((priv_fd = open(fifoname, O_RDONLY)) < 0) {
        // If an error occurs report a failure and delete the private fifo
        perror("Cannot open private FIFO for reading!");
        printLog(c_msg.i, getpid(), pthread_self(), dur, -1, FAILED);
        if (unlink(fifoname) < 0) {
            perror("Cannot delete private FIFO");
        }
        return NULL;
    }

    // Read server response
    char server_response[MAX_MSG];
    int tries = 0;
    while (read(priv_fd, server_response, MAX_MSG) <= 0 && tries < 5) {
        // If an error occurs, try again after a few instants to account for possible server delay
        usleep(300);
        tries++;
    } 

    if (tries == 5) {
        // If maximum number of tries reached, report a failure and delete private fifo
        printLog(c_msg.i, getpid(), pthread_self(), dur, -1, FAILED);
        close(priv_fd);
        if (unlink(fifoname) < 0) {
            perror("Cannot delete private FIFO");
        }
        return NULL;
    }

    // Parse server response into struct
    struct fifo_msg s_msg;
    sscanf(server_response, "[ %d, %d, %lu, %d, %d ]", &s_msg.i, &s_msg.pid, &s_msg.tid, &s_msg.dur, &s_msg.pl);

    // Determine if request was successful or not based on response content
    if (s_msg.dur == -1 && s_msg.pl == -1) {
        printLog(c_msg.i, getpid(), pthread_self(), dur, -1, CLOSED);
        closed = 1;
    } else {
        printLog(c_msg.i, getpid(), pthread_self(), dur, s_msg.pl, IAMIN);
    }

    // Delete private fifo
    close(priv_fd);
    if (unlink(fifoname) < 0) {
        perror("Cannot delete private FIFO");
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    setStart();

    struct Uarg args = processArgs(argc,argv); // Process console arguments

    int i = 0; // Initializes request id

    // Request-generating loop, stops when server is closed or runtime has passed
    while (elapsedTime() < args.numberSeconds && !closed) {
        struct client_msg c_msg = {args.fifoname, i}; // Creates message struct

        // Creates thread for sending request
        pthread_t t;
        pthread_create(&t, NULL, requestSender, &c_msg);
        
        i++; // Increments request id
        usleep(50e3); // Waits 50ms before generating next request
    }

    pthread_exit(0);
}
