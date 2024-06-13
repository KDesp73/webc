#include <string.h>
#include <stdlib.h>
#define CLIB_IMPLEMENTATION
#include "clib.h"

#define MAX_BUFFER_SIZE 1024
char* file;

void Append(const char* text){
    size_t new_size = strlen(file) + strlen(CONCAT(text, "\n")) + 1;
    file = (char*) realloc(file, new_size);
    strcat(file, CONCAT(text, "\n"));
}

void HtmlInit(const char* lang)
{
    file = (char*) malloc(MAX_BUFFER_SIZE);
    file[0] = '\0';

    CstrArray init = clib_cstr_array_make(
        "<!DOCTYPE html>",
        CONCAT("<html lang=\"", lang, "\">"),
        NULL
    );

    char* temp = (char*) clib_cstr_array_join("\n", init);
    file = (char*) realloc(file, strlen(temp) + 1);
    strcpy(file, temp);
    free(temp);
}

void Header(const char* title)
{  
    CstrArray header = clib_cstr_array_make(
        "<head>",
        "<meta charset=\"UTF-8\">",
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">",
        CONCAT("<title>", title, "</title>"),
        "</head>\n",
        NULL
    );

    char* temp = (char*) clib_cstr_array_join("\n", header);
    Append(temp);
    free(temp);
}

void Body(const char* first, ...)
{
    Append("<body>");
    va_list args;
    va_start(args, first);
    const char* text = first;
    while (text != NULL) {
        Append(text);
        text = va_arg(args, const char*);
    }
    va_end(args);
    Append("</body>");
}

void Clean()
{
    free(file);
}

void Export()
{
    Cstr html_file = "index.html";
    clib_write_file(html_file, file, "w");
    INFO("%s created", html_file);
}

int main(void)
{
    HtmlInit("en");
    Header("Test");
    Body(
        "<h1>Hello, World!</h1>", 
        "<p>This is a test paragraph.</p>",
        NULL
    );

    Export();
    Clean();
    return 0;
}

