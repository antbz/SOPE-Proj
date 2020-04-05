#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH 512

struct sduarg {
    int all, bytes, deref, sepdir, Bsize, max_depth;
    char path[MAX_PATH];
};

struct sduarg process_args(int argc, char* argv[]);

void print_args(struct sduarg args);
