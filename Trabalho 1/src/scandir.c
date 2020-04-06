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
        
        if (args->deref) {
            if (stat(path, &buf)) {
                perror(dirent->d_name);
                logExit(4);
            }
        } else if (lstat(path, &buf)) {
            perror(dirent->d_name);
            logExit(4);
        }
        
        int size;

        if (S_ISREG(buf.st_mode) || S_ISLNK(buf.st_mode)) {
            if (args->bytes) {
                size = buf.st_size;
            } else {
                size = buf.st_blocks * 512 / args->Bsize;
            }
            printf("%d\t%s\n", size, path);
        }
    }
}