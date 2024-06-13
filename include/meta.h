#ifndef META_H
#define META_H
#include "attribute.h"

typedef enum {
    CHARSET,
    NAME,
    CONTENT,
    REL,
    HREF,
    PROPERTY,
    HTTP_EQUIV,
} MetaTagAttr;

typedef struct {
    Attribute** attributes;
    size_t count;
} MetaTag;

MetaTag* MakeMetaTag(Attribute* first, ...);
Cstr MetaTagToString(MetaTag* tag);

#endif // META_H

