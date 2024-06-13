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

void HtmlStart(char** buffer, Cstr lang);
void HtmlEnd(char** buffer);
void Head(char** buffer, Cstr title, Tag* first, ...);
void BodyEnd(char** buffer);
void BodyStart(char** buffer);
void Paragraph(char** buffer, Cstr text);
void Heading(char** buffer, size_t size, Cstr text);

#endif // TAGS_H
