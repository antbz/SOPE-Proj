#include "sduarg.h"

struct sduarg process_args(int argc, char* argv[]) {
    struct sduarg args = {0, 0, 0, 0, -1, -1, ""};
    
    for (int i = 1; i < argc; i++) {
        char* arg = argv[i];
        if (arg[0] == '-') {
            if (strlen(arg) == 2) {
                if (strcmp(arg, "-a") == 0) { args.all = 1; }
                else if (strcmp(arg, "-b") == 0) { args.bytes = 1; }
                else if (strcmp(arg, "-L") == 0) { args.deref = 1; }
                else if (strcmp(arg, "-S") == 0) { args.sepdir = 1; }
                else if (strcmp(arg, "-l") == 0) { continue; }
                else if (strcmp(arg, "-B") == 0) {
                    i++;
                    int size = atoi(argv[i]);
                    if (size < 1) {
                        printf("Error invalid arguments! size not provided or invalid.\n\n");
                        printf("Usage: %s -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]\n", argv[0]);
                        exit(1);
                    } else {
                        args.Bsize = size;
                    }
                }
                else {
                    printf("Error invalid arguments!\n\n");
                    printf("Usage: %s -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]\n", argv[0]);
                    exit(1);
                }
            } else if (strlen(arg) > 2) {
                if (strcmp(arg, "--all") == 0) { args.all = 1; }
                else if (strcmp(arg, "--bytes") == 0) { args.bytes = 1; }
                else if (strcmp(arg, "--dereference") == 0) { args.deref = 1; }
                else if (strcmp(arg, "--separate-dirs") == 0) { args.sepdir = 1; }
                else if (strcmp(arg, "--count-links") == 0) { continue; }
                else if (strncmp(arg, "--block-size=", 13) == 0) {
                    arg = strtok(arg, "=");
                    arg = strtok(NULL, "");
                    printf("after= %s\n", arg);
                    int size = atoi(arg);
                    if (size < 1) {
                        printf("Error invalid arguments! size not provided or invalid.\n\n");
                        printf("Usage: %s -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]\n", argv[0]);
                        exit(1);
                    } else {
                        args.Bsize = size;
                    }
                } else if (strncmp(arg, "--max-depth=", 12) == 0) {
                    arg = strtok(arg, "=");
                    arg = strtok(NULL, "");
                    printf("after= %s\n", arg);
                    if (strcmp(arg, "0") == 0) { 
                        args.max_depth = 0; 
                    }
                    else {
                        int max_depth = atoi(arg);
                        if (max_depth < 1) {
                            printf("Error invalid arguments! max_depth not provided or invalid.\n\n");
                            printf("Usage: %s -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]\n", argv[0]);
                            exit(1);
                        } else {
                            args.max_depth = max_depth;
                        }
                    }
                } else {
                    printf("Error invalid arguments!\n\n");
                    printf("Usage: %s -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]\n", argv[0]);
                    exit(1);
                }
            } else {
                printf("Error invalid arguments!\n\n");
                printf("Usage: %s -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]\n", argv[0]);
                exit(1);
            }
        } else if (args.path[0] == '\0') {
            if (strlen(arg) > MAX_PATH) {
                printf("Error invalid arguments! Path too long!\n\n");
                printf("Usage: %s -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]\n", argv[0]);
                exit(1);
            }
            strcpy(args.path, arg);
        } else {
            printf("Error invalid arguments! More than one path provided!\n\n");
            printf("Usage: %s -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]\n", argv[0]);
            exit(1);
        }
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
