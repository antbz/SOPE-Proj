#include "sduarg.h"
#include "exreg.h"

struct sduarg process_args(int argc, char* argv[]) {
    struct sduarg args = {0, 0, 0, 0, 1024, -1, ""}; // Default values for variables
    
    for (int i = 1; i < argc; i++) { // Go through all arguments, ignoring the 1st (program name)
        char* arg = argv[i]; // Store argument for use later
        
        if (arg[0] == '-') { // Analyse arguments starting with '-' (option arguments)
            if (strlen(arg) == 2) { // If argument length is 2, it's an -[opt] option argument
                if (strcmp(arg, "-a") == 0) { args.all = 1; }
                else if (strcmp(arg, "-b") == 0) { args.bytes = 1; }
                else if (strcmp(arg, "-L") == 0) { args.deref = 1; }
                else if (strcmp(arg, "-S") == 0) { args.sepdir = 1; }
                else if (strcmp(arg, "-l") == 0) { continue; } // "-l" is always on, so no variable needs to be set. Still, it's a valid argument
                else if (strcmp(arg, "-B") == 0) { // "-B" preceeds a number specifying the block size, so extra checks needed
                    i++; // Moves to next argument
                    int size = atoi(argv[i]);
                    if (size < 1) { // Block size can never be less than 1. If argument following "-B" is not a valid number, atoi returns 0, so this also detects invalid input
                        printf("Error invalid arguments! size not provided or invalid.\n\n");
                        printf("Usage: %s -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]\n", argv[0]);
                        logExit(1);
                    } else { // If block size is valid, set it
                        args.Bsize = size;
                    }
                }
                else { // No other length 2 option arguments exist, so if it's not one of them, throw an error
                    printf("Error invalid arguments!\n\n");
                    printf("Usage: %s -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]\n", argv[0]);
                    logExit(1);
                }
            } else if (strlen(arg) > 2) { // If argument length is >2, it's a --[option] argument or a --[option]=N argument
                if (strcmp(arg, "--all") == 0) { args.all = 1; }
                else if (strcmp(arg, "--bytes") == 0) { args.bytes = 1; }
                else if (strcmp(arg, "--dereference") == 0) { args.deref = 1; }
                else if (strcmp(arg, "--separate-dirs") == 0) { args.sepdir = 1; }
                else if (strcmp(arg, "--count-links") == 0) { continue; }
                else if (strncmp(arg, "--block-size=", 13) == 0) { // --[option]=N arguments need extra checks
                    arg = strtok(arg, "=");
                    arg = strtok(NULL, ""); // These split the string using '=' as the delimiter, setting arg to what's after the delimiter
                    int size = atoi(arg);
                    if (size < 1) { // If <1 or not a number, throw an error
                        printf("Error invalid arguments! size not provided or invalid.\n\n");
                        printf("Usage: %s -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]\n", argv[0]);
                        logExit(1);
                    } else {
                        args.Bsize = size;
                    }
                } else if (strncmp(arg, "--max-depth=", 12) == 0) {
                    arg = strtok(arg, "=");
                    arg = strtok(NULL, "");
                    if (strcmp(arg, "0") == 0) { // max_depth can be 0. Since atoi does not distinguish 0 from a non integer string, this check is needed
                        args.max_depth = 0; 
                    }
                    else {
                        int max_depth = atoi(arg);
                        if (max_depth < 1) { // If it's not 0, it can only be larger than 1 or it is invalid
                            printf("Error invalid arguments! max_depth not provided or invalid.\n\n");
                            printf("Usage: %s -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]\n", argv[0]);
                            logExit(1);
                        } else {
                            args.max_depth = max_depth;
                        }
                    }
                } else { // If it is not one of the --[option] arguments, throw an error
                    printf("Error invalid arguments!\n\n");
                    printf("Usage: %s -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]\n", argv[0]);
                    logExit(1);
                }
            } else { // If the length of the argument is <2, throw an error
                printf("Error invalid arguments!\n\n");
                printf("Usage: %s -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]\n", argv[0]);
                logExit(1);
            }
        } else if (args.path[0] == '\0') { // If argument is not option argument and no path has yet been defined, define path
            if (strlen(arg) > MAX_PATH) {
                printf("Error invalid arguments! Path too long!\n\n");
                printf("Usage: %s -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]\n", argv[0]);
                logExit(1);
            }
            strcpy(args.path, arg);
            if (args.path[strlen(args.path) - 1] != '/') { strcat(args.path, "/"); }
        } else { // If argument is not option argument and path has already been defined, argument is not valid, throw error
            printf("Error invalid arguments! More than one path provided!\n\n");
            printf("Usage: %s -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]\n", argv[0]);
            logExit(1);
        }
    }

    if (args.path[0] == '\0') { // If no path specified, default to current path
        strcpy(args.path, "./");
    }

    return args;
}

void print_args(struct sduarg args) {
    printf("all: %d\n", args.all);
    printf("bytes: %d\n", args.bytes);
    printf("deref: %d\n", args.deref);
    printf("sepdir: %d\n", args.sepdir);
    printf("Bsize: %d\n", args.Bsize);
    printf("max-depth: %d\n", args.max_depth);
    printf("path: %s\n", args.path);
}
