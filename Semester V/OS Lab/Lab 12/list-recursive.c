#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void list_files(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror(path);
        return;
    }

    struct dirent *entry;
    char fullpath[1024];

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

        struct stat statbuf;
        if (stat(fullpath, &statbuf) == -1) {
            perror(fullpath);
            continue;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            list_files(fullpath);
        } else {
            printf("%s\n", fullpath);
        }
    }

    closedir(dir);
}

int main() {
    list_files(".");
    return 0;
}
