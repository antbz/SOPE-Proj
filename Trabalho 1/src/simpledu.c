#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sduarg.h"
#include "exreg.h"
#include "scandir.h"

int main(int argc, char* argv[]) {
    initReg();

    logCreate(argc, argv);

    struct sduarg args = process_args(argc, argv);

    scan_dir(&args);
    
    logExit(0);
}
