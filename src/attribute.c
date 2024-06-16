#include "webc.h"
#include <stdarg.h>
#include <stdio.h>
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

WEBCAPI AttributeList MakeAttributeList(Attribute* first, ...)
{
    AttributeList result = {0};
    result.count = 0;
    if(first == NULL) return result;

    va_list args;
    va_start(args, first);
    result.count++;
    for (Attribute* next = va_arg(args, Attribute*); next != NULL; next = va_arg(args, Attribute*)) {
        result.count++;
    }
    va_end(args);

    result.items = (Attribute**) malloc(sizeof(result.items[0]) * result.count);
    if (result.items == NULL) {
        PANIC("could not allocate memory: %s", strerror(errno));
    }

    va_start(args, first);
    result.items[0] = first;
    for (size_t i = 1; i < result.count; ++i) {
        Attribute* next = va_arg(args, Attribute*);
        result.items[i]= next;
    }
    va_end(args);

    return result; 
}


WEBCAPI AttributeList ModifierToAttributeList(Modifier modifier)
{
    AttributeList list = {0};

    // Count the number of set attributes
    if (modifier.style != NULL) list.count++;
    if (modifier.href != NULL) list.count++;
    if (modifier.id != NULL) list.count++;
    if (modifier.class != NULL) list.count++;
    if (modifier.target != NULL) list.count++;
    if (modifier.src != NULL) list.count++;
    if (modifier.alt != NULL) list.count++;
    if (modifier.width > 0) list.count++;
    if (modifier.height > 0) list.count++;

    list.items = (Attribute**) malloc(sizeof(list.items[0]) * list.count);
    list.count = 0;

    if(modifier.style != NULL)
        list.items[list.count++] = MakeAttribute(ATTR_STYLE, modifier.style);

    if(modifier.href != NULL)
        list.items[list.count++] = MakeAttribute(ATTR_HREF, modifier.href);
    
    if(modifier.id != NULL)
        list.items[list.count++] = MakeAttribute(ATTR_ID, modifier.id);
    
    if(modifier.class != NULL)
        list.items[list.count++] = MakeAttribute(ATTR_CLASS, modifier.class);
    
    if(modifier.target != NULL)
        list.items[list.count++] = MakeAttribute(ATTR_TARGET, modifier.target);

    if(modifier.src != NULL)
        list.items[list.count++] = MakeAttribute(ATTR_SRC, modifier.src);

    if(modifier.alt != NULL)
        list.items[list.count++] = MakeAttribute(ATTR_ALT, modifier.alt);

    if(modifier.width > 0)
        list.items[list.count++] = MakeAttribute(ATTR_WIDTH, clib_format_text("%zu", modifier.width));

    if(modifier.height > 0)
        list.items[list.count++] = MakeAttribute(ATTR_HEIGHT, clib_format_text("%zu", modifier.height));

    return list;
}
