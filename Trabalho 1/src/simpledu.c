#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sduarg.h"
#include "exreg.h"

int main(int argc, char* argv[]) {
    initReg();

    logCreate(argc, argv);

    struct sduarg args = process_args(argc, argv);

    print_args(args);
    printf("time: %f\n", elapsed_time());
    logExit(2);
    return 0;
}
