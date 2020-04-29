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

#define MAX_THREAD 100

int pl = 1;
pthread_mutex_t pl_mut = PTHREAD_MUTEX_INITIALIZER;

void* handleRequest(void* arg) {
    struct fifo_msg c_msg;
    sscanf((char*) arg, "[ %d, %d, %lu, %d, %d ]", &c_msg.i, &c_msg.pid, &c_msg.tid, &c_msg.dur, &c_msg.pl);

    printLog(c_msg.i, c_msg.pid, c_msg.tid, c_msg.dur, c_msg.pl, RECEIVED);

    char fifoname[MAX_FIFONAME];
    sprintf(fifoname, "/tmp/%d.%ld", c_msg.pid, c_msg.tid);

    int priv_fd;

    if ((priv_fd = open(fifoname, O_WRONLY)) < 0) {
        perror("Cannot open FIFO for WRITING!");
        printLog(c_msg.i, getpid(), pthread_self(), c_msg.dur, -1, GAVEUP);
        return NULL;
    }

    pthread_mutex_lock(&pl_mut);
    int place = pl;
    pl++;
    pthread_mutex_unlock(&pl_mut);

    char response[1024];
    sprintf(response, "[ %d, %d, %ld, %d, %d ]", c_msg.i, getpid(), pthread_self(), c_msg.dur, place);
    write(priv_fd, response, strlen(response));
    close(priv_fd);

    printLog(c_msg.i, getpid(), pthread_self(), c_msg.dur, place, ENTER);

    usleep(c_msg.dur * 1e3);

    printLog(c_msg.i, getpid(), pthread_self(), c_msg.dur, place, TIMEUP);

    return NULL;
}

void sendTooLate(char* arg) {
    struct fifo_msg c_msg;
    sscanf(arg, "[ %d, %d, %lu, %d, %d ]", &c_msg.i, &c_msg.pid, &c_msg.tid, &c_msg.dur, &c_msg.pl);

    char fifoname[MAX_FIFONAME];
    sprintf(fifoname, "/tmp/%d.%ld", c_msg.pid, c_msg.tid);

    int priv_fd;

    if ((priv_fd = open(fifoname, O_WRONLY)) < 0) {
        perror("Cannot open FIFO for WRITING!");
        printLog(c_msg.i, getpid(), pthread_self(), c_msg.dur, -1, GAVEUP);
        return;
    }

    char response[1024];
    sprintf(response, "[ %d, %d, %ld, %d, %d ]", c_msg.i, getpid(), pthread_self(), -1, -1);
    write(priv_fd, response, strlen(response));
    close(priv_fd);

    printLog(c_msg.i, getpid(), pthread_self(), -1, -1, TOOLATE);
}

int main(int argc, char* argv[]) {
    setStart();

    struct Qarg args = processArgs(argc,argv);
    int fifo_fd;

    if (mkfifo(args.fifoname, 0660) == -1) {
        printf("Error creating FIFO\n");
        exit(2);
    }

    printf("Server FIFO created successfully!\n");

    if ((fifo_fd = open(args.fifoname, O_RDONLY | O_NONBLOCK)) < 0) {
        perror("Cannot open FIFO for reading!");
        if (unlink(args.fifoname) < 0) {
            perror("Cannot delete FIFO");
        }
        printf("FIFO deleted successfully\n");
        exit(3);
    }

    printf("Server opened FIFO for READING!\n");

    char msg[1024];
    while (elapsedTime() < args.numberSeconds) {
        read_msg(fifo_fd, msg, ']');
        if (msg[0] == '[') {
            char msg_cpy[1024];
            strcpy(msg_cpy, msg);

            pthread_t t;
            pthread_create(&t, NULL, handleRequest, msg_cpy);
            pthread_detach(t);

            strcpy(msg, "");
        }
    }

    if (read_msg(fifo_fd, msg, ']') > 0 && msg[0] == '[') {
        sendTooLate(msg);
    }

    if (unlink(args.fifoname) < 0) {
        perror("Cannot delete FIFO");
    }
    printf("FIFO deleted successfully\n");

    exit(0);
}