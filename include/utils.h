#ifndef UTILS_H
#define UTILS_H

#define CLIB_IMPLEMENTATION
#include "clib.h"

void Append(char** file, Cstr text);
void Clean(char* file);
void Export(char* file, Cstr path);

#endif // UTILS_H
