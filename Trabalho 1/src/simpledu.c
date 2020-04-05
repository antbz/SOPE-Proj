#include <stdio.h>
#include "sdu_arg.h"

int main(int argc, char* argv[]) {
    struct sduarg args;
    for (int i = 1; i < argc; i++) {
        args.path = "Poopies\n";
        printf("%s", args.path);
    }
    return 0;
}
