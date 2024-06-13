#include "attribute.h"
#include <stdlib.h>
#include <string.h>

Attribute* MakeAttribute(int name, const char* value)
{
    Attribute* attr = (Attribute*)malloc(sizeof(Attribute));
    if (attr == NULL) {
        return NULL;
    }

    attr->name = name;

    attr->value = (char*)malloc((strlen(value) + 1) * sizeof(char));
    if (attr->value == NULL) {
        free(attr);
        return NULL;
    }
    strcpy(attr->value, value);

    return attr;
}

Cstr AttributeNameToString(AttributeName attr)
{
    switch (attr) {
        case CHARSET:
            return "charset";
        case NAME:
            return "name";
        case CONTENT:
            return "content";
        case REL:
            return "rel";
        case HREF:
            return "href";
        case PROPERTY:
            return "property";
        case HTTP_EQUIV:
            return "http_equiv";
        case STYLE:
            return "style";
        case TITLE:
            return "title";
        case TARGET:
            return "target";
        default:
            return NULL;
    }
}

Attribute** MakeAttributeList(Attribute* first, ...)
{
    if(first == NULL) return NULL;

    Attribute** attributes;
    size_t count = 0;

    count += 1;

    va_list args;
    va_start(args, first);
    for (Attribute* next = va_arg(args, Attribute*); next != NULL; next = va_arg(args, Attribute*)) {
        count += 1;
    }
    va_end(args);

    attributes = (Attribute**) malloc(sizeof(attributes[0]) * count);
    if (attributes == NULL) {
        PANIC("could not allocate memory: %s", strerror(errno));
    }
    count = 0;

    attributes[count++] = first;

    va_start(args, first);
    for (Attribute* next = va_arg(args, Attribute*); next != NULL; next = va_arg(args, Attribute*)) {
        attributes[count++] = next;
    }
    va_end(args);

    return attributes; 
}
