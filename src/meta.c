#include "meta.h"
#include "attribute.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>


Cstr MetaTagToString(MetaTag* tag){

}

MetaTag* MakeMetaTag(Attribute* first, ...){
    MetaTag* tag = (MetaTag*) malloc(sizeof(MetaTag));

    if(first == NULL) return NULL;

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
