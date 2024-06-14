#include "webc.h"

WEBCAPI void Append(char** buffer, Cstr text)
{
    if(text == NULL) return;

    size_t new_size = strlen(*buffer) + strlen(CONCAT(text, "\n")) + 1;
    *buffer = (char*) realloc(*buffer, new_size);
    strcat(*buffer, CONCAT(text, "\n"));
}

WEBCAPI void Export(char* buffer, Cstr path)
{
    clib_write_file(path, buffer, "w");
    INFO("%s created", path);
}

WEBCAPI void ExportRoute(Route route)
{
    char* buffer = (char*) route.route();
    Export(buffer, route.path);
    Clean(&buffer);
}

WEBCAPI void Clean(char** buffer)
{
    free(*buffer);
}
