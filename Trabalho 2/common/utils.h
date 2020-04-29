#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define IWANT "IWANT"
#define RECEIVED "RECVD"
#define ENTER "ENTER"
#define IAMIN "IAMIN"
#define TIMEUP "TIMUP"
#define TOOLATE "2LATE"
#define CLOSED "CLOSD"
#define FAILED "FAILD"
#define GAVEUP "GAVUP"

#define MAX_FIFONAME 512

struct fifo_msg {
    int i;
    int pid;
    unsigned long tid;
    int dur;
    int pl;
};

void printLog(int i, int pid, long tid, int dur, int pl, char* oper);

void setStart();

double elapsedTime();

int read_msg(int fifo_fd, char* msg, char delim);
