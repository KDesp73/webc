/**
 * MIT License
 * 
 * Copyright (c) 2024 Konstantinos Despoinidis
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Written by: KDesp73 (despoinidisk@gmail.com)
 */

#ifndef WEBC_H
#define WEBC_H

#define VERSION_MAJOR 0
#define VERSION_MINOR 0
#define VERSION_PATCH 1
#define VERSION "0.0.1"

#define WEBCAPI 

// ############ Includes  ############ //

#define CLIB_IMPLEMENTATION
#include "clib.h"

// ############ Constants ############ //

#define MAX_BUFFER_SIZE 1024

// ############ Types ############ //

typedef enum {
    ATTR_CHARSET = 0,
    ATTR_NAME,
    ATTR_CONTENT,
    ATTR_REL,
    ATTR_HREF,
    ATTR_PROPERTY,
    ATTR_HTTP_EQUIV,
    ATTR_STYLE,
    ATTR_TITLE,
    ATTR_TARGET,
    ATTR_SRC,
    ATTR_ALT,
    ATTR_WIDTH,
    ATTR_HEIGHT,
    ATTR_ATTRIBUTE_NAME_COUNT
} AttributeName;

typedef struct {
    AttributeName name; 
    char* value;
} Attribute;

typedef struct {
    Cstr name;
    Attribute** attributes;
    size_t attr_count;
    size_t attr_capacity;
} Tag;

// ############ Tags ############ //

WEBCAPI Tag* MakeTag(Cstr name, Attribute* first, ...);
WEBCAPI Cstr ClosingTag(Tag* tag);
WEBCAPI Cstr TagToString(Tag* tag);

#define OPENING_TAG(name) CONCAT("<", name, ">")
#define CLOSING_TAG(name) CONCAT("</", name, ">")

// ############ Utils ############ //

WEBCAPI void Append(char** buffer, Cstr text);
WEBCAPI void Clean(char** buffer);
WEBCAPI void Export(char* buffer, Cstr path);

// ############ Attributes ############ //

WEBCAPI Attribute* MakeAttribute(int name, Cstr value);
WEBCAPI Attribute** MakeAttributeList(Attribute* first, ...);
WEBCAPI Cstr AttributeNameToString(AttributeName attr);


// ############ Elements ############ //

WEBCAPI void Head(char** buffer, Cstr title, Tag* first, ...);

WEBCAPI void HtmlStart(char** buffer, Cstr lang);
WEBCAPI void HtmlEnd(char** buffer);
WEBCAPI void BodyEnd(char** buffer);
WEBCAPI void BodyStart(char** buffer);
WEBCAPI void ScriptStart(char** buffer);
WEBCAPI void ScriptEnd(char** buffer);
WEBCAPI void StyleStart(char** buffer);
WEBCAPI void StyleEnd(char** buffer);

// Helpers
WEBCAPI void Block(char** buffer, Tag* tag, void(* func)(char**));
WEBCAPI void BlockAttr(char** buffer, Cstr name, Attribute** attributes, void(* func)(char**));
WEBCAPI void InlineBlock(char** buffer, Cstr name, Attribute** attributes, Cstr text);

WEBCAPI void Abbr(char** buffer, Attribute** attributes, Cstr abbr);
WEBCAPI void Address(char** buffer, Attribute** attributes, void(* func)(char**));
WEBCAPI void Anchor(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void AnchorEx(char** buffer, Attribute** attributes, void(* func)(char**));
WEBCAPI void Blockquote(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Bold(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Cite(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Code(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Del(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Div(char** buffer, Attribute** attributes, void(* func)(char**));
WEBCAPI void Heading(char** buffer, Attribute** attributes, size_t size, Cstr text);
WEBCAPI void Paragraph(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void ParagraphEx(char** buffer, Attribute** attributes, void(* func)(char**));
WEBCAPI void Li(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void LiEx(char** buffer, Attribute** attributes, void(* func)(char**));
WEBCAPI void Ul(char** buffer, Attribute** attributes, void(* func)(char**));
WEBCAPI void Ol(char** buffer, Attribute** attributes, void(* func)(char**));

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


#endif // WEBC_H
