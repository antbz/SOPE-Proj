#include "inputreader.h"


struct Qarg processArgs(int argc, char* argv[]){
    struct Qarg args = {0, ""};

    if (argc != 4) { // The number of arguments is always 4 (program_name, -t, nsecs and fifoname)
        printf("Error! Invalid arguments.\nUsage: %s <-t nsecs> fifoname\n", argv[0]);
        exit(1);
    }

    for (int i = 1; i < argc; i++) { // Process arguments ignoring program name
        char* arg = argv[i];

        if (arg[0] == '-') { // Process option parameters
            if (strcmp(arg, "-t") == 0) { // -t option
                i++;
                int nsecs = atoi(argv[i]); 
                if (nsecs < 1) { // must always be a positive value
                    printf("Error! Invalid arguments. nsecs parameter must be positive.\nUsage: %s <-t nsecs> fifoname\n", argv[0]);
                    exit(1);
                } else {
                    args.numberSeconds = nsecs;
                }
            } else { // The only valid option parameter is -t
                printf("Error! Invalid arguments.\nUsage: %s <-t nsecs> fifoname\n", argv[0]);
                exit(1);
            }
        } else if (args.fifoname[0] == '\0') { // If parameter not an option parameter and no fifoname provided yet
            if (strlen(arg) > MAX_FIFONAME) {
                printf("Error! Invalid arguments. fifoname too long.\nUsage: %s <-t nsecs> fifoname\n", argv[0]);
                exit(1);
            }
            strcpy(args.fifoname, arg);
        } else { // Too many fifonames
            printf("Error! Invalid arguments. More than one fifoname provided.\nUsage: %s <-t nsecs> fifoname\n", argv[0]);
            exit(1);
        }
    }

    return args;
}

void print_Qargs(struct Qarg args) {
    printf("nsecs: %d\n", args.numberSeconds);
    printf("fifoname: %s\n", args.fifoname);
}
