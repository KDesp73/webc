#include "utils.h"
#include "clib.h"

void Append(char** file, Cstr text)
{
    if(text == NULL) return;

    size_t new_size = strlen(*file) + strlen(CONCAT(text, "\n")) + 1;
    *file = (char*) realloc(*file, new_size);
    strcat(*file, CONCAT(text, "\n"));
}

void Export(char* file, Cstr path)
{
    clib_write_file(path, file, "w");
    INFO("%s created", path);
}

void Clean(char* file)
{
    free(file);
}
