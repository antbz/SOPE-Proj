#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/utils.h"

/**
 * @brief struct for holding client program arguments
*/
struct Uarg {
    unsigned numberSeconds; /**< Maximum program runtime */
    char fifoname[MAX_FIFONAME]; /**< Provided public fifo name */
};

/**
 * @brief parses a given array of console arguments into a struct
 * 
 * @return Uarg struct with corresponding values
*/
struct Uarg processArgs(int argc, char* argv[]);


/**
 * @brief [DEBUG] prints values in Uarg struct
*/
void print_Uargs(struct Uarg args);
