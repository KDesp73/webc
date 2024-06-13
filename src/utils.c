#include "utils.h"
#include "clib.h"

void Append(char** buffer, Cstr text)
{
    if(text == NULL) return;

    size_t new_size = strlen(*buffer) + strlen(CONCAT(text, "\n")) + 1;
    *buffer = (char*) realloc(*buffer, new_size);
    strcat(*buffer, CONCAT(text, "\n"));
}

void Export(char* buffer, Cstr path)
{
    clib_write_file(path, buffer, "w");
    INFO("%s created", path);
}

void Clean(char** buffer)
{
    free(*buffer);
}
