#include "webc-core.h"
#include <stdio.h>
#include "webc-md.h"

WEBCAPI char* WEBC_BufferInit()
{
    char* buffer = (char*) malloc(1);
    memset(buffer, 0, 1);

    return buffer;
}

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

int is_markdown_file(const char *path) {
    const char *markdownExtension = ".md";
    const char *fileExtension = strrchr(path, '.');

    if (fileExtension != NULL && strcmp(fileExtension, markdownExtension) == 0) {
        return 1; // It is a Markdown file
    }

    return 0; // It is not a Markdown file
}

WEBCAPI void WEBC_IntegrateFile(char** buffer, Cstr path)
{
    char* file = NULL;
    if(is_url(path)){
        char* command = clib_format_text("curl -fsSL %s", path);
        file = clib_execute_command(command);
        free(command);

        if(file == NULL){
            ERRO("Couldn't get url: %s", path);
        }
    } else if (is_markdown_file(path)){
        file = (char*) WEBC_MarkdownToHtml(path);

        if(file == NULL){
            ERRO("Couldn't convert markdown file: %s", path);
        }
    } else {
        file = clib_read_file(path);

        if(file == NULL){
            ERRO("Couldn't read file: %s", path);
        }
    }
    
    WEBC_AppendLn(buffer, file);
    free(file);
}
