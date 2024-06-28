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

int is_url(const char *str)
{
    if (strncmp(str, "http://", 7) == 0 || strncmp(str, "https://", 8) == 0) {
        if (strchr(str + 7, '/') != NULL || strchr(str + 8, '/') != NULL) {
            return 1; // Valid URL
        }
    }
    return 0; // Not a valid URL
}

WEBCAPI void WEBC_IntegrateFile(char** buffer, Cstr path)
{
    if(is_url(path)){
        char* command = clib_format_text("curl -fsSL %s", path);
        Cstr out = clib_execute_command(command);
        free(command);

        if(out == NULL){
            WEBC_Clean(buffer);
            PANIC("Couldn't get url: %s", path);
        }

        WEBC_AppendLn(buffer, out);
        free((char*) out);
    } else {
        Cstr contents = clib_read_file(path);

        if(contents == NULL){
            WEBC_Clean(buffer);
            PANIC("Couldn't read file: %s", path);
        }

        WEBC_AppendLn(buffer, contents);
        free((char*) contents);
    }
}
