#include "webc-core.h"
#include <stdio.h>

WEBCAPI void WEBC_AppendLn(char** buffer, Cstr text)
{
    assert(buffer != NULL && *buffer != NULL);
    assert(text != NULL);

    Cstr new_text = clib_format_text("%s\n", text);
    size_t new_size = strlen(*buffer) + strlen(new_text) + 1;
    *buffer = (char*) realloc(*buffer, new_size);
    if (*buffer == NULL) {
        PANIC("Failed to reallocate memory");
    }
    strcat(*buffer, new_text);
    free((char*) new_text);
}

WEBCAPI void WEBC_Append(char** buffer, const char* text) {
    assert(buffer != NULL && *buffer != NULL);
    assert(text != NULL);

    size_t current_size = strlen(*buffer);
    size_t text_len = strlen(text);

    *buffer = (char*) realloc(*buffer, current_size + text_len + 1);
    if (*buffer == NULL) {
        PANIC("Failed to reallocate memory");
    }

    strcat(*buffer, text);
}

WEBCAPI void WEBC_Clean(char** buffer)
{
    free(*buffer);
    *buffer = NULL;
}

WEBCAPI void WEBC_IntegrateFile(char** buffer, Cstr path)
{
    Cstr contents = clib_read_file(path);
    
    if(contents == NULL){
        PANIC("Couldn't read file: %s", path);
    }

    WEBC_AppendLn(buffer, contents);
}
