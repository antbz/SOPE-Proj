#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

/**
 * Operation constants
*/
#define IWANT "IWANT"
#define RECEIVED "RECVD"
#define ENTER "ENTER"
#define IAMIN "IAMIN"
#define TIMEUP "TIMUP"
#define TOOLATE "2LATE"
#define CLOSED "CLOSD"
#define FAILED "FAILD"
#define GAVEUP "GAVUP"

/**
 * Maximum length of fifo names
*/
#define MAX_FIFONAME 512

/**
 * Maximum lenght of fifo messages
*/
#define MAX_MSG 512

/**
 * @brief struct holding fifo message parameters
*/
struct fifo_msg {
    int i; /**< request sequential id */
    int pid; /**< sending process' PID */
    unsigned long tid; /**< sending thread's TID */
    int dur; /**< request duration */
    int pl; /**< assigned place */
};

/**
 * @brief registers an operation in STDOUT
*/
void printLog(int i, int pid, long tid, int dur, int pl, char* oper);

/**
 * @brief saves current time in global variable start
*/
void setStart();

/**
 * @brief calculates elapsed time between saved start time and curent time
*/
double elapsedTime();

/**
 * @brief reads a message from specified file descriptor to given buffer, until a
 * specific character is read
 * 
 * @param fifo_fd file descriptor
 * @param msg pointer to buffer
 * @param delim separating character
 * 
 * @return number of characters read from fifo_fd
*/
int read_msg(int fifo_fd, char* msg, char delim);
