#include "scandir.h"

void scan_dir(struct sduarg *args) {
    DIR* dir;
    struct dirent* dirent;
    struct stat buf;

    if ((dir = opendir(args->path)) == NULL) {
        perror(args->path);
        logExit(3);
    }

    while ((dirent = readdir(dir)) != NULL) {
        char path[MAX_PATH];
        strcpy(path, args->path);
        strcat(path, dirent->d_name);
        if (stat(path, &buf)) {
            perror(dirent->d_name);
            logExit(4);
        }

        printf("%s - %d\n", path, S_ISDIR(buf.st_mode));
    }
}