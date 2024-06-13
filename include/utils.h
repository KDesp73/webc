#ifndef UTILS_H
#define UTILS_H

#define CLIB_IMPLEMENTATION
#include "clib.h"

void Append(char** buffer, Cstr text);
void Clean(char** buffer);
void Export(char* buffer, Cstr path);

#endif // UTILS_H
