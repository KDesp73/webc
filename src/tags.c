#include "webc.h"
#include <stdlib.h>

WEBCAPI void CleanTag(Tag *tag)
{
    for(size_t i = 0; i < tag->attr_count; ++i){
        free(tag->attributes[i]);
    }
    free(tag->attributes);
    free(tag);
}

WEBCAPI Cstr ClosingTag(Tag* tag)
{
    return clib_format_text("</%s>", tag->name);
}

WEBCAPI Cstr TagToString(Tag* tag)
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

WEBCAPI Tag* MakeTag(Cstr name, Attribute* first, ...)
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

 

