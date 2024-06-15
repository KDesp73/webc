#include "webc.h"
#include <stdlib.h>

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
    clib_write_file(path, buffer, "w");
    INFO("%s created", path);
}

WEBCAPI void ExportRoute(Route route)
{
    Export(route.buffer, route.path);
    Clean(&route.buffer);
}

WEBCAPI void Clean(char** buffer)
{
    free(*buffer);
}
