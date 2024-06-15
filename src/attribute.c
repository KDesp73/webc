#include "webc.h"
#include <stdlib.h>
#include <string.h>

WEBCAPI Attribute* MakeAttribute(AttributeName name, const char* value)
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

WEBCAPI Cstr AttributeNameToString(AttributeName attr)
{
    switch (attr) {
        case ATTR_ID:
            return "id";
        case ATTR_CLASS:
            return "class";
        case ATTR_CHARSET:
            return "charset";
        case ATTR_NAME:
            return "name";
        case ATTR_CONTENT:
            return "content";
        case ATTR_REL:
            return "rel";
        case ATTR_HREF:
            return "href";
        case ATTR_PROPERTY:
            return "property";
        case ATTR_HTTP_EQUIV:
            return "http_equiv";
        case ATTR_STYLE:
            return "style";
        case ATTR_TITLE:
            return "title";
        case ATTR_TARGET:
            return "target";
        case ATTR_SRC:
            return "src";
        case ATTR_ALT:
            return "alt";
        case ATTR_WIDTH:
            return "width";
        case ATTR_HEIGHT:
            return "height";
        default:
            return NULL;
    }
}

WEBCAPI Attribute** MakeAttributeList(Attribute* first, ...)
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
