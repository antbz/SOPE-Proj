#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exreg.h"

#define MAX_PATH 256

/**
 * @brief struct holding values of argument options
*/
struct sduarg {
    int all, bytes, deref, sepdir, Bsize, max_depth;
    char path[MAX_PATH];
};

/**
 * @brief returns a struct with adequate argument values
*/
struct sduarg process_args(int argc, char* argv[]);

/**
 * @brief [DEBUG] prints values in args struct
*/
void print_args(struct sduarg args);
