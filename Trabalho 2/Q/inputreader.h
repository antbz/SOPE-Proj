#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/utils.h"

#define DEFAULT_MAX_THREAD 300

/**
 * @brief struct for holding server program arguments
*/
struct Qarg {
    unsigned numberSeconds; /**< Maximum program runtime */
    char fifoname[MAX_FIFONAME]; /**< Provided public fifo name */
    unsigned nplaces; /**< Maximum number of bathroom places */
    unsigned nthreads; /**< Maxumum number of request-handling threads */
};

/**
 * @brief parses a given array of console arguments into a struct
 * 
 * @return Qarg struct with corresponding values
*/
struct Qarg processArgs(int argc, char* argv[]);

/**
 * @brief [DEBUG] prints values in Qarg struct
*/
void print_Qargs(struct Qarg args);
