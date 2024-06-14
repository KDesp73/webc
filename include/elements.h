#ifndef ELEMENTS_H
#define ELEMENTS_H

#include "attribute.h"
#define CLIB_IMPLEMENTATION
#include "clib.h"
#include "tags.h"

void Head(char** buffer, Cstr title, Tag* first, ...);

void HtmlStart(char** buffer, Cstr lang);
void HtmlEnd(char** buffer);
void BodyEnd(char** buffer);
void BodyStart(char** buffer);
void ScriptStart(char** buffer);
void ScriptEnd(char** buffer);
void StyleStart(char** buffer);
void StyleEnd(char** buffer);

// Helpers
void Block(char** buffer, Tag* tag, void(* func)(char**));
void BlockAttr(char** buffer, Cstr name, Attribute** attributes, void(* func)(char**));
void InlineBlock(char** buffer, Cstr name, Attribute** attributes, Cstr text);

void Abbr(char** buffer, Attribute** attributes, Cstr abbr);
void Address(char** buffer, Attribute** attributes, void(* func)(char**));
void Anchor(char** buffer, Attribute** attributes, Cstr text);
void AnchorEx(char** buffer, Attribute** attributes, void(* func)(char**));
void Blockquote(char** buffer, Attribute** attributes, Cstr text);
void Bold(char** buffer, Attribute** attributes, Cstr text);
void Cite(char** buffer, Attribute** attributes, Cstr text);
void Code(char** buffer, Attribute** attributes, Cstr text);
void Del(char** buffer, Attribute** attributes, Cstr text);
void Div(char** buffer, Attribute** attributes, void(* func)(char**));
void Heading(char** buffer, Attribute** attributes, size_t size, Cstr text);
void Paragraph(char** buffer, Attribute** attributes, Cstr text);
void ParagraphEx(char** buffer, Attribute** attributes, void(* func)(char**));
void Li(char** buffer, Attribute** attributes, Cstr text);
void LiEx(char** buffer, Attribute** attributes, void(* func)(char**));
void Ul(char** buffer, Attribute** attributes, void(* func)(char**));
void Ol(char** buffer, Attribute** attributes, void(* func)(char**));



#define PlainText(buffer, text) \
    Append(buffer, text)
#define Javascript(buffer, text) \
    PlainText(buffer, text)
#define Input(buffer, attr, ...) \
    Append(buffer, TagToString(MakeTag("input", attr, ##__VA_ARGS__)))
#define Img(buffer, attr, ...) \
    Append(buffer, TagToString(MakeTag("img", attr, ##__VA_ARGS__)))
#define Meta(buffer, attr, ...) \
    Append(buffer, TagToString(MakeTag("meta", attr, ##__VA_ARGS__)))
#define Link(buffer, attr, ...) \
    Append(buffer, TagToString(MakeTag("link", attr, ##__VA_ARGS__)))
#define Br(buffer) \
    Append(buffer, TagToString(MakeTag("br", NULL)))
#define Hr(buffer) \
    Append(buffer, TagToString(MakeTag("hr", NULL)))

#endif //ELEMENTS_H
