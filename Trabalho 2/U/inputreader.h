#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/utils.h"

struct Uarg {
    unsigned numberSeconds; //The maximum execution time of the program
    char fifoname[MAX_FIFONAME]; //The name of the fifo
};

struct Uarg processArgs(int argc, char* argv[]);

void print_Uargs(struct Uarg args);
