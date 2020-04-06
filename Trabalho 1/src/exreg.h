#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define DEFAULT_LOG "log.txt"
#define MAX_LOG_LINE 512

/**
 * @brief opens log file in LOG_FILENAME (or DEFAULT_LOG if not defined)
*/
void initReg();

/**
 * @brief returns double with elapsed CPU time in ms since start of program
*/
double elapsed_time();

/**
 * @brief prints a log line to opened log file
*/
void printLog(double instant, pid_t pid, char* action, char* info);

/**
 * @brief logs an exit with specified status
*/
void logExit(int status);

/**
 * @brief logs a process creation with specified arguments
*/
void logCreate(int argc, char* argv[]);