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
    int fifo_fd;
    int i;
    int dur;
};

int closed = 0;

void* requestSender(void* arg) {
    struct client_msg c_msg = *(struct client_msg*) arg;
    char msg[1024];

    sprintf(msg, "[ %d, %d, %ld, %d, %d ]", c_msg.i, getpid(), pthread_self(), c_msg.dur, -1);

    if (write(c_msg.fifo_fd, &msg, strlen(msg)) < 0) {
        printLog(c_msg.i, getpid(), pthread_self(), c_msg.dur, -1, FAILED);
        closed = 1;
        return NULL;
    } else {
        printLog(c_msg.i, getpid(), pthread_self(), c_msg.dur, -1, IWANT);
    }

    char fifoname[MAX_FIFONAME];
    sprintf(fifoname, "/tmp/%d.%ld", getpid(), pthread_self());

    if (mkfifo(fifoname, 0660) == -1) {
        printf("Error creating private FIFO\n");
        printLog(c_msg.i, getpid(), pthread_self(), c_msg.dur, -1, FAILED);
        return NULL;
    }

    int priv_fd;

    if ((priv_fd = open(fifoname, O_RDONLY | O_NONBLOCK)) < 0) {
        perror("Cannot open FIFO for reading!");
        printLog(c_msg.i, getpid(), pthread_self(), c_msg.dur, -1, FAILED);
        if (unlink(fifoname) < 0) {
            perror("Cannot delete FIFO");
        }
        printf("FIFO deleted successfully\n");
        return NULL;
    }

    char server_response[1024];
    int tries = 0;

    while (read(priv_fd, server_response, 1024) <= 0 && tries < 5) {
        usleep(200);
        tries++;
    } 

    if (tries == 5) {
        printLog(c_msg.i, getpid(), pthread_self(), c_msg.dur, -1, FAILED);
        if (unlink(fifoname) < 0) {
            perror("Cannot delete FIFO");
        }
        printf("FIFO deleted successfully\n");

        return NULL;
    }

    struct fifo_msg s_msg;

    sscanf(server_response, "[ %d, %d, %lu, %d, %d ]", &s_msg.i, &s_msg.pid, &s_msg.tid, &s_msg.dur, &s_msg.pl);

    if (s_msg.dur == -1 && s_msg.pl == -1) {
        printLog(c_msg.i, getpid(), pthread_self(), c_msg.dur, -1, CLOSED);
        closed = 1;
    } else {
        printLog(c_msg.i, getpid(), pthread_self(), c_msg.dur, s_msg.pl, IAMIN);
    }

    if (unlink(fifoname) < 0) {
        perror("Cannot delete FIFO");
    }
    printf("FIFO deleted successfully\n");

    return NULL;
}

int main(int argc, char* argv[]) {
    setStart();

    struct Uarg args = processArgs(argc,argv);
    int fifo_fd;

    if ((fifo_fd = open(args.fifoname, O_WRONLY | O_NONBLOCK)) < 0) {
        perror("Cannot open server FIFO for WRITING");
        exit(2);
    }

    printf("Server FIFO successfully opened for WRITING!\n");

    int i = 0;

    while (elapsedTime() < args.numberSeconds && !closed) {
        int dur = rand() % 800 + 200;
        struct client_msg c_msg = {fifo_fd, i, dur};

        pthread_t t;
        pthread_create(&t, NULL, requestSender, &c_msg);
        pthread_join(t, NULL);

        i++;
        usleep(5e3);
    }

    close(fifo_fd);

    printf("All clients done!\n");

    exit(0);
}