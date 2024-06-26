#include "webc-core.h"
#include <stdlib.h>

WEBCAPI void CleanTag(Tag** tag)
{
    if (*tag == NULL) {
        return;
    }

    for (size_t i = 0; i < (*tag)->attributes.count; ++i) {
        free((*tag)->attributes.items[i]);
        (*tag)->attributes.items[i] = NULL;
    }
    free((*tag)->attributes.items);
    (*tag)->attributes.items = NULL;
    free(*tag);
    *tag = NULL;
}

WEBCAPI Cstr ClosingTag(Tag* tag)
{
    return clib_format_text("</%s>", tag->name);
}

WEBCAPI Cstr TagToString(Tag* tag)
{
    assert(tag != NULL);
    assert(tag->name != NULL);

    char* tag_str = (char*)malloc(1);
    if (tag_str == NULL) {
        return NULL;
    }
    tag_str[0] = '\0';

    Append(&tag_str, "<");
    Append(&tag_str, tag->name);

    for (size_t i = 0; i < tag->attributes.count; ++i) {
        Cstr name = AttributeNameToString(tag->attributes.items[i]->name);
        Cstr value = tag->attributes.items[i]->value;
        if(name == NULL) {
            PANIC("Null name for name: %d, Ensure that you terminated the AttributeList", tag->attributes.items[i]->name);
        }

        char* attr_str = NULL;
        if(value == NULL)
            attr_str = clib_format_text(" %s", name);
        else 
            attr_str = clib_format_text(" %s=\"%s\"", name, value);

        if(attr_str == NULL) continue;

        Append(&tag_str, attr_str);

        free(attr_str);
    }

    Append(&tag_str, ">");
    return tag_str;
}

WEBCAPI Tag* MakeTag(Cstr name, AttributeList attributes)
{
    Tag* tag = (Tag*) malloc(sizeof(Tag));
    tag->name = name;
    tag->attributes = attributes;

    return tag;
}

WEBCAPI Tag* MakeTagAttr(Cstr name, Attribute* first, ...)
{
    Tag* tag = (Tag*) malloc(sizeof(Tag));
    tag->attributes.count= 0;
    tag->name = name;

    if(first == NULL) return tag;


    tag->attributes.count += 1;

    va_list args;
    va_start(args, first);
    for (Attribute* next = va_arg(args, Attribute*); next != NULL; next = va_arg(args, Attribute*)) {
        tag->attributes.count += 1;
    }
    va_end(args);

    tag->attributes.items = (Attribute**) malloc(sizeof(tag->attributes.items[0]) * tag->attributes.count);
    if (tag->attributes.items == NULL) {
        PANIC("could not allocate memory: %s", strerror(errno));
    }
    tag->attributes.count = 0;

    tag->attributes.items[tag->attributes.count++] = first;

    va_start(args, first);
    for (Attribute* next = va_arg(args, Attribute*); next != NULL; next = va_arg(args, Attribute*)) {
        tag->attributes.items[tag->attributes.count++] = next;
    }
    va_end(args);

    return tag;
}

 

