#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH 256

/**
 * @brief struct holding values of argument options
*/
struct sduarg {
    int all; /**< Show file information, */
    int bytes; /**< Show size in bytes. */
    int deref; /**< Dereference symbolic links. */
    int sepdir; /**< Do not include subdirectory size.*/
    int Bsize; /**< Block size. */
    int blocks; /**< Show size in blocks. */
    int max_depth; /**< Maximum directory depth, */
    char path[MAX_PATH]; /**< Path to directory. */
};

/**
 * @brief returns a struct with adequate argument values
*/
struct sduarg process_args(int argc, char* argv[]);

/**
 * @brief [DEBUG] prints values in args struct
*/
void print_args(struct sduarg args);
