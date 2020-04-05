#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sduarg.h"

int main(int argc, char* argv[]) {
    struct sduarg args = process_args(argc, argv);
    
    print_args(args);
    
    return 0;
}
