#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <sys/wait.h>
#include "sduarg.h"
#include "exreg.h"

/**
 * @brief scans dir contents and prints information as specified in args
*/
void scan_dir(struct sduarg args);
