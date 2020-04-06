#include "scandir.h"

void scan_dir(struct sduarg *args) {
    DIR* dir;
    struct dirent* dirent;
    // struct stat buf;

    if ((dir = opendir(args->path)) == NULL) {
        perror(args->path);
        logExit(3);
    }

    while ((dirent = readdir(dir)) != NULL) {
        printf("%s\n", dirent->d_name);
    }
}