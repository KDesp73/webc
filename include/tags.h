#ifndef TAGS_H
#define TAGS_H

#include "attribute.h"
#define CLIB_IMPLEMENTATION
#include "clib.h"
#include "utils.h"

typedef struct {
    Cstr name;
    Attribute** attributes;
    size_t attr_count;
    size_t attr_capacity;
} Tag;

Tag* MakeTag(Cstr name, Attribute* first, ...);
#define OPENING_TAG(name) CONCAT("<", name, ">")
#define CLOSING_TAG(name) CONCAT("</", name, ">")
Cstr ClosingTag(Tag* tag);
Cstr TagToString(Tag* tag);



#endif // TAGS_H
