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
        default:
            return NULL;
    }
}
