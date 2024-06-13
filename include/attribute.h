#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#define CLIB_IMPLEMENTATION
#include "clib.h"
typedef struct {
    int name; 
    Cstr value;
} Attribute;

#endif // ATTRIBUTE_H
