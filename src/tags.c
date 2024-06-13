#include "tags.h"
#include "clib.h"
#include "config.h"
#include "utils.h"
#include <stdlib.h>

Cstr ClosingTag(Tag* tag)
{
    return clib_format_text("</%s>", tag->name);
}

Cstr TagToString(Tag* tag)
{
    assert(tag != NULL);
    assert(tag->name != NULL);

    int initial_size = 256;
    char* tag_str = (char*)malloc(initial_size * sizeof(char));
    if (tag_str == NULL) {
        return NULL;
    }

    strcpy(tag_str, CONCAT("<", tag->name));
    size_t current_size = strlen(tag_str);

    for (size_t i = 0; i < tag->count; ++i) {
        Cstr name = AttributeNameToString(tag->attributes[i]->name);
        Cstr value = tag->attributes[i]->value;
        if(name == NULL) {
            ERRO("Null name for name=%d", tag->attributes[i]->name);
            continue;
        }

        if(value == NULL) {
            ERRO("Null value for name=%d", tag->attributes[i]->name);
            continue;
        }

        char* attr_str = clib_format_text(" %s=\"%s\"", name, value);
        size_t attr_len = strlen(attr_str);

        if ((int) (current_size + attr_len) >= initial_size) {
            initial_size *= 2;
            tag_str = (char*)realloc(tag_str, initial_size * sizeof(char));
            if (tag_str == NULL) {
                free(attr_str);
                return NULL;
            }
        }

        strcat(tag_str, attr_str);
        current_size += attr_len;

        free(attr_str);
    }

    return CONCAT(tag_str, ">");
}


Tag* MakeTag(Cstr name, Attribute* first, ...)
{
    Tag* tag = (Tag*) malloc(sizeof(Tag));
    tag->count = 0;
    tag->name = name;

    if(first == NULL) return tag;


    tag->count += 1;

    va_list args;
    va_start(args, first);
    for (Attribute* next = va_arg(args, Attribute*); next != NULL; next = va_arg(args, Attribute*)) {
        tag->count += 1;
    }
    va_end(args);

    tag->attributes = (Attribute**) malloc(sizeof(tag->attributes[0]) * tag->count);
    if (tag->attributes == NULL) {
        PANIC("could not allocate memory: %s", strerror(errno));
    }
    tag->count = 0;

    tag->attributes[tag->count++] = first;

    va_start(args, first);
    for (Attribute* next = va_arg(args, Attribute*); next != NULL; next = va_arg(args, Attribute*)) {
        tag->attributes[tag->count++] = next;
    }
    va_end(args);

    return tag;
}

 

void Head(char** buffer, Cstr title, Tag* first, ...)
{
    CstrArray header_arr = clib_cstr_array_make(
        "<head>",
        "<meta charset=\"UTF-8\">",
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">",
        NULL
    );

    char* header = (char*) clib_cstr_array_join("\n", header_arr);

    if (first != NULL) {
        char* first_tag_str = (char*) TagToString(first);
        strcat(header, "\n");
        strcat(header, first_tag_str);
        free(first_tag_str);

        va_list args;
        va_start(args, first);
        Tag* tag = va_arg(args, Tag*);
        while (tag != NULL) {
            char* tag_str = (char*) TagToString(tag);
            strcat(header, "\n");
            strcat(header, tag_str);
            free(tag_str);
            tag = va_arg(args, Tag*);
        }
        va_end(args);
    }

    Cstr title_tag = clib_format_text("<title>%s</title>", title);
    Cstr header_str = clib_format_text("%s\n%s\n</head>", header, title_tag);
    Append(buffer, header_str);

    free(header);
}

void HtmlStart(char**buffer, Cstr lang)
{
    if(lang == NULL){
        PANIC("Language is NULL");
    }

    *buffer = (char*) malloc(MAX_BUFFER_SIZE);
    if(*buffer == NULL){
        PANIC("Couldnt initialize buffer");
    }
    *buffer[0] = '\0';

    char* temp = (char*) JOIN("\n",
        "<!DOCTYPE html>",
        CONCAT("<html lang=\"", lang, "\">\n")
    );

    *buffer = (char*) realloc(*buffer, strlen(temp) + 1);
    if(*buffer == NULL){
        PANIC("Couldnt reallocate buffer");
    }

    strcpy(*buffer, temp);
    free(temp);
}

void HtmlEnd(char** buffer)
{
    Append(buffer, CLOSE_TAG("html"));
}


void BodyStart(char** buffer)
{
    Append(buffer, "<body>");
}

void BodyEnd(char** buffer)
{
    Append(buffer, "</body>");
}

void Heading(char** buffer, size_t size, Cstr text){
    if(size > 6) {
        PANIC("Heading size should be between 1 and 6");
    }

    Cstr tag_name = clib_format_text("h%zu", size);
    Append(buffer, clib_format_text("%s%s</%s>", TagToString(MakeTag(tag_name, NULL)), text, tag_name));
}

void Paragraph(char** buffer, Cstr text)
{
    Append(buffer, clib_format_text("<p>%s</p>", text));
}

