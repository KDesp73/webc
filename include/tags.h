#ifndef TAGS_H
#define TAGS_H

#include "attribute.h"
#define CLIB_IMPLEMENTATION
#include "clib.h"
#include "utils.h"

typedef struct {
    Cstr name;
    Attribute** attributes;
    size_t count;
} Tag;

Tag* MakeTag(Cstr name, Attribute* first, ...);
#define CLOSE_TAG(name) CONCAT("</", name, ">")
Cstr ClosingTag(Tag* tag);
Cstr TagToString(Tag* tag);

void HtmlInit(char** file, Cstr lang);
void Header(char** file, Cstr title, Tag* first, ...);
void BodyEnd(char** file);
void BodyStart(char** file);
void Paragraph(char** file, Cstr text);
void Heading(char** file, size_t size, Cstr text);

#endif // TAGS_H
