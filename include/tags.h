#ifndef TAGS_H
#define TAGS_H

#include "attribute.h"
#include <stdint.h>
#define CLIB_IMPLEMENTATION
#include "clib.h"
#include "utils.h"

typedef struct {
    Cstr name;
    Attribute** attributes;
    size_t attr_count;
    size_t attr_capacity;
} Tag;

Tag* MakeTag(Cstr name, Attribute* first, ...);
#define OPENING_TAG(name) CONCAT("<", name, ">")
#define CLOSING_TAG(name) CONCAT("</", name, ">")
Cstr ClosingTag(Tag* tag);
Cstr TagToString(Tag* tag);

void Head(char** buffer, Cstr title, Tag* first, ...);

void HtmlStart(char** buffer, Cstr lang);
void HtmlEnd(char** buffer);
void BodyEnd(char** buffer);
void BodyStart(char** buffer);


void InlineBlock(char** buffer, Cstr name, Attribute** attributes, Cstr text);
void Block(char** buffer, Tag* tag, void(* func)(char**));
void BlockAttr(char** buffer, Cstr name, Attribute** attributes, void(* func)(char**));
void Anchor(char** buffer, Attribute** attributes, Cstr text);
void AnchorEx(char** buffer, Attribute** attributes, void(* func)(char**));
void Abbr(char** buffer, Attribute** attributes, Cstr abbr);
void Paragraph(char** buffer, Attribute** attributes, Cstr text);
void ParagraphEx(char** buffer, Attribute** attributes, void(* func)(char**));
void Address(char** buffer, Attribute** attributes, void(* func)(char**));
void Div(char** buffer, Attribute** attributes, void(* func)(char**));
void Heading(char** buffer, Attribute** attributes, size_t size, Cstr text);
void Bold(char** buffer, Attribute** attributes, Cstr text);
void Blockquote(char** buffer, Attribute** attributes, Cstr text);
void Cite(char** buffer, Attribute** attributes, Cstr text);
void Code(char** buffer, Attribute** attributes, Cstr text);
void Del(char** buffer, Attribute** attributes, Cstr text);



#define PlainText(buffer, text) \
    Append(buffer, text)
#define Input(buffer, attr, ...) \
    Append(buffer, TagToString(MakeTag("input", attr, ##__VA_ARGS__)))
#define Br(buffer) \
    Append(buffer, TagToString(MakeTag("br", NULL)))
#define Hr(buffer) \
    Append(buffer, TagToString(MakeTag("hr", NULL)))


#endif // TAGS_H
