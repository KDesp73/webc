#include "webc-core.h"

WEBCAPI void Append(char** buffer, Cstr text)
{
    if(text == NULL) return;

    Cstr new_text = CONCAT(text, "\n");
    size_t new_size = strlen(*buffer) + strlen(new_text) + 1;
    *buffer = (char*) realloc(*buffer, new_size);
    strcat(*buffer, new_text);
}

WEBCAPI void Clean(char** buffer)
{
    free(*buffer);
    *buffer = NULL;
}
