#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#define CLIB_IMPLEMENTATION
#include "clib.h"

typedef enum {
    CHARSET = 0,
    NAME,
    CONTENT,
    REL,
    HREF,
    PROPERTY,
    HTTP_EQUIV,
    STYLE,
    TITLE,
    TARGET,
    ATTRIBUTE_NAME_COUNT
} AttributeName;

typedef struct {
    int name; 
    char* value;
} Attribute;

Attribute* MakeAttribute(int name, Cstr value);
Attribute** MakeAttributeList(Attribute* first, ...);
Cstr AttributeNameToString(AttributeName attr);

#endif // ATTRIBUTE_H
