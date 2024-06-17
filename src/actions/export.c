#include "webc-actions.h"
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void CreatePath(const char *path) {
    char *token;
    char *path_copy = strdup(path);
    char *current_dir = NULL;

    token = strtok(path_copy, "/");
    while (token != NULL) {
        if (current_dir == NULL) {
            current_dir = strdup(token);
        } else {
            current_dir = realloc(current_dir, strlen(current_dir) + strlen(token) + 2);
            strcat(current_dir, "/");
            strcat(current_dir, token);
        }

        struct stat st = {0};
        if (stat(current_dir, &st) == -1) {
            mkdir(current_dir, 0700);
        }

        token = strtok(NULL, "/");
    }

    free(path_copy);
    free(current_dir);
}

WEBCAPI void Export(char* buffer, Cstr path)
{
    CreatePath(path);
    Cstr n_path = NULL;
    
    if (path[strlen(path) - 1] == PATH_SEP[0]) 
        n_path = CONCAT(path, "index.html");
    else
        n_path = CONCAT(path, "/index.html");

    clib_write_file(n_path, buffer, "w");
    Clean(&buffer);
    INFO("%s created", n_path);
}

WEBCAPI void ExportRoute(Route route)
{
    Export(route.buffer, route.path);
}
