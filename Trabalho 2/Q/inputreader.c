#include "inputreader.h"


struct Qarg processArgs(int argc, char* argv[]){
    struct Qarg args = {0, "", 0, 0};

    if (argc < 4 || argc > 8) { // The number of arguments is at least 4 at maximum 8 (program_name, -t, nsecs, fifoname, [-l], [nplaces], [-n], [nthreads])
        printf("Error! Invalid arguments.\nUsage: %s <-t nsecs> [-l nplaces] [-n nsecs] fifoname\n", argv[0]);
        exit(1);
    }

    for (int i = 1; i < argc; i++) { // Process arguments ignoring program name
        char* arg = argv[i];

        if (arg[0] == '-') { // Process option parameters
            if (strcmp(arg, "-t") == 0) { // -t option - nsecs
                i++;
                int nsecs = atoi(argv[i]); 
                if (nsecs < 1) { // must always be a positive value
                    printf("Error! Invalid arguments. nsecs parameter must be positive.\nUsage: %s <-t nsecs> [-l nplaces] [-n nsecs] fifoname\n", argv[0]);
                    exit(1);
                } else {
                    args.numberSeconds = nsecs;
                }
            } else if (strcmp(arg, "-l") == 0) { // -l option - nplaces
                i++;
                int nplaces = atoi(argv[i]); 
                if (nplaces < 1) { // must always be a positive value
                    printf("Error! Invalid arguments. nplaces parameter must be positive.\nUsage: %s <-t nsecs> [-l nplaces] [-n nsecs] fifoname\n", argv[0]);
                    exit(1);
                } else {
                    args.nplaces = nplaces;
                }
            } else if (strcmp(arg, "-n") == 0) { // -n option - nthreads
                i++;
                int nthreads = atoi(argv[i]); 
                if (nthreads < 1) { // must always be a positive value
                    printf("Error! Invalid arguments. nthreads parameter must be positive.\nUsage: %s <-t nsecs> [-l nplaces] [-n nsecs] fifoname\n", argv[0]);
                    exit(1);
                } else {
                    args.nthreads = nthreads;
                }
            } else { // The only valid option parameters are -t -l -n
                printf("Error! Invalid arguments.\nUsage: %s <-t nsecs> [-l nplaces] [-n nsecs] fifoname\n", argv[0]);
                exit(1);
            }
        } else if (args.fifoname[0] == '\0') { // If parameter not an option parameter and no fifoname provided yet
            if (strlen(arg) > MAX_FIFONAME) {
                printf("Error! Invalid arguments. fifoname too long.\nUsage: %s <-t nsecs> [-l nplaces] [-n nsecs] fifoname\n", argv[0]);
                exit(1);
            }
            strcpy(args.fifoname, arg);
        } else { // Too many fifonames
            printf("Error! Invalid arguments. More than one fifoname provided.\nUsage: %s <-t nsecs> [-l nplaces] [-n nsecs] fifoname\n", argv[0]);
            exit(1);
        }
    }

    if (args.fifoname[0] == '\0') {
        printf("Error! No FIFO name provided.\nUsage: %s <-t nsecs> [-l nplaces] [-n nsecs] fifoname\n", argv[0]);
        exit(1);
    } else if (args.numberSeconds == 0) {
        printf("Error! No number of seconds provided.\nUsage: %s <-t nsecs> [-l nplaces] [-n nsecs] fifoname\n", argv[0]);
        exit(1);
    }

    return args;
}

void print_Qargs(struct Qarg args) {
    printf("nsecs: %d\n", args.numberSeconds);
    printf("fifoname: %s\n", args.fifoname);
    printf("nplaces: %d\n", args.nplaces);
    printf("nthreads: %d\n", args.nthreads);
}
