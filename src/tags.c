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

    for (size_t i = 0; i < tag->attr_count; ++i) {
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
    tag->attr_count = 0;
    tag->name = name;

    if(first == NULL) return tag;


    tag->attr_count += 1;

    va_list args;
    va_start(args, first);
    for (Attribute* next = va_arg(args, Attribute*); next != NULL; next = va_arg(args, Attribute*)) {
        tag->attr_count += 1;
    }
    va_end(args);

    tag->attributes = (Attribute**) malloc(sizeof(tag->attributes[0]) * tag->attr_count);
    if (tag->attributes == NULL) {
        PANIC("could not allocate memory: %s", strerror(errno));
    }
    tag->attr_count = 0;

    tag->attributes[tag->attr_count++] = first;

    va_start(args, first);
    for (Attribute* next = va_arg(args, Attribute*); next != NULL; next = va_arg(args, Attribute*)) {
        tag->attributes[tag->attr_count++] = next;
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
    Append(buffer, CLOSING_TAG("html"));
}


void BodyStart(char** buffer)
{
    Append(buffer, "<body>");
}

void BodyEnd(char** buffer)
{
    Append(buffer, "</body>");
}

void Heading(char** buffer, Attribute** attributes, size_t size, Cstr text){
    if(size == 0 || size > 6) {
        PANIC("Heading size should be between 1 and 6");
    }

    Tag* tag = (Tag*) malloc(sizeof(Tag));
    tag->name = clib_format_text("h%zu", size);
    tag->attributes = attributes;

    tag->attr_count = 0;
    if(attributes != NULL)
        while(attributes[tag->attr_count] != NULL) tag->attr_count++;
    tag->attr_capacity = tag->attr_count;

    Append(buffer, clib_format_text("%s%s%s", TagToString(tag), text, ClosingTag(tag)));
}

void Paragraph(char** buffer, Attribute** attributes, Cstr text)
{
    Tag* tag = (Tag*) malloc(sizeof(Tag));
    tag->name = "p";
    tag->attributes = attributes;

    tag->attr_count = 0;
    if(attributes != NULL)
        while(attributes[tag->attr_count] != NULL) tag->attr_count++;
    tag->attr_capacity = tag->attr_count;

    Append(buffer, clib_format_text("%s%s%s", TagToString(tag), text, ClosingTag(tag)));
}
void ParagraphEx(char** buffer, Attribute** attributes, void(* func)(char**))
{
    Tag* tag = (Tag*) malloc(sizeof(Tag));
    tag->name = "p";
    tag->attributes = attributes;
    tag->attr_count = 0;
    if(attributes != NULL)
        while(attributes[tag->attr_count] != NULL) tag->attr_count++;
    tag->attr_capacity = tag->attr_count;

    Block(buffer, tag, func);
}


void Abbr(char** buffer, Attribute** attributes, Cstr abbr)
{
    Tag* tag = (Tag*) malloc(sizeof(Tag));
    tag->name = "abbr";
    tag->attributes = attributes;
    tag->attr_count = 0;
    if(attributes != NULL)
        while(attributes[tag->attr_count] != NULL) tag->attr_count++;
    tag->attr_capacity = tag->attr_count;

    Append(buffer, clib_format_text("%s%s%s", TagToString(tag), abbr, ClosingTag(tag)));
}

void Block(char** buffer, Tag* tag, void(* func)(char**))
{
    assert(tag != NULL);

    Append(buffer, TagToString(tag));
    func(buffer);
    Append(buffer, ClosingTag(tag));
}

void Address(char** buffer, Attribute** attributes, void(* func)(char**))
{
    Tag* tag = (Tag*) malloc(sizeof(Tag));
    tag->name = "address";
    tag->attributes = attributes;
    tag->attr_count = 0;
    if(attributes != NULL)
        while(attributes[tag->attr_count] != NULL) tag->attr_count++;
    tag->attr_capacity = tag->attr_count;

    Block(buffer, tag, func);
}

void Div(char** buffer, Attribute** attributes, void(* func)(char**))
{
    Tag* tag = (Tag*) malloc(sizeof(Tag));
    tag->name = "div";
    tag->attributes = attributes;
    tag->attr_count = 0;
    if(attributes != NULL)
        while(attributes[tag->attr_count] != NULL) tag->attr_count++;
    tag->attr_capacity = tag->attr_count;

    Block(buffer, tag, func);
}
