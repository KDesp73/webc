#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define CLIB_IMPLEMENTATION
#include "clib.h"

#define VERSION "0.0.1"
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

void BodyStart()
{
    Append("<body>");
}

void BodyEnd()
{
    Append("</body");
}

// TODO: Add to clib.h
char* format_text(const char *format, ...) {
    va_list args;
    va_start(args, format);

    int size = vsnprintf(NULL, 0, format, args) + 1; // +1 for the null terminator

    va_end(args);

    char *formatted_string = (char*)malloc(size);
    if (formatted_string == NULL) {
        return NULL;
    }

    va_start(args, format);
    vsnprintf(formatted_string, size, format, args);

    va_end(args);

    return formatted_string;
}

void Heading(size_t size, Cstr text){
    if(size > 6) {
        PANIC("Heading size should be between 1 and 6");
    }

    Append(CONCAT(format_text("<h%zu>", size), text, format_text("</h%zu>")));
}

void Clean()
{
    free(file);
}

void Export(Cstr path)
{
    clib_write_file(path, file, "w");
    INFO("%s created", path);
}

int main(int argc, char** argv)
{
    Cstr output = "index.html";
    HtmlInit("en");
    Header("Test");

    BodyStart();
        Heading(1, "Heading 1");
        Heading(2, "Heading 2");
        Heading(3, "Heading 3");
        Heading(4, "Heading 4");
        Heading(5, "Heading 5");
        Heading(6, "Heading 6");
    BodyEnd();

    Export(output);
    Clean();
    return 0;
}

