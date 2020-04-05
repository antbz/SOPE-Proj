#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sduarg.h"
#include "exreg.h"

int main(int argc, char* argv[]) {
    initReg();

    struct sduarg args = process_args(argc, argv);
    
    printf("User: %s\n", getenv("LOG_FILENAME"));
    print_args(args);
    
    return 0;
}
