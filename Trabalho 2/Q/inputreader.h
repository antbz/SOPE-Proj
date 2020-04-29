#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FIFONAME 512

struct Qarg {
    unsigned numberSeconds; //The maximum execution time of the program
    char fifoname[MAX_FIFONAME]; //The name of the fifo
};

struct Qarg processArgs(int argc, char* argv[]);

void print_Qargs(struct Qarg args);
