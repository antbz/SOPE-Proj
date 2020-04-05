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

int initReg();

double elapsed_time();

void printLog(double instant, pid_t pid, char* action, char* info);

void logExit(int status);
