#include "webc.h"
#include <stdlib.h>
#include <sys/stat.h>

// Not in webc.h
WEBCAPI void CreatePath(const char *path) {
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

WEBCAPI void Append(char** buffer, Cstr text)
{
    if(text == NULL) return;

    Cstr new_text = CONCAT(text, "\n");
    size_t new_size = strlen(*buffer) + strlen(new_text) + 1;
    *buffer = (char*) realloc(*buffer, new_size);
    strcat(*buffer, new_text);
}

WEBCAPI void Export(char* buffer, Cstr path)
{
    CreatePath(path);
    Cstr n_path = CONCAT(path, "index.html");

    clib_write_file(n_path, buffer, "w");
    Clean(&buffer);
    INFO("%sindex.html created", path);
}

WEBCAPI void ExportRoute(Route route)
{
    Export(route.buffer, route.path);
}

WEBCAPI void Clean(char** buffer)
{
    free(*buffer);
    *buffer = NULL;
}
