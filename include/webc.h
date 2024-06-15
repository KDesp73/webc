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

#ifndef WEBCAPI
    #define WEBCAPI extern // Functions defined as 'extern' by default (implicit specifiers)
#endif

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
    ATTR_ATTRIBUTE_NAME_COUNT,
    ATTR_ID,
    ATTR_CLASS
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

typedef struct {
    Cstr path;
    char* buffer;
}Route;

typedef void (*BlockContents)(char** buffer);
// ############ Tags ############ //

/**
 * Creates a Tag struct pointer
 *
 * @param name The tage name
 * @param first The first Attribute struct pointer for the tag
 * @param ... The rest of the Attribute struct pointers
 *
 * @return Tag*
 */
WEBCAPI Tag* MakeTag(Cstr name, Attribute* first, ...);

/**
 * Creates a closing tag (</>) for any Tag struct pointer
 *
 * @param tag The Tag struct pointer
 *
 * @return Cstr
 */
WEBCAPI Cstr ClosingTag(Tag* tag);

/**
 * Creates the opening tag (<name attr1="value" attr2="value">) for any Tag struct pointer
 *
 * @param tag The Tag struct pointer
 *
 * @return Cstr
 */
WEBCAPI Cstr TagToString(Tag* tag);

WEBCAPI void CleanTag(Tag* tag);

#define OPENING_TAG(name) CONCAT("<", name, ">")
#define CLOSING_TAG(name) CONCAT("</", name, ">")

// ############ Utils ############ //

/**
 * Appends text (and a newline) to the buffer
 *
 * @param buffer The buffer's pointer
 * @param text The text to append
 */
WEBCAPI void Append(char** buffer, Cstr text);

/**
 * Frees the buffer's pointer
 *
 * @param buffer The buffer to free
 */
WEBCAPI void Clean(char** buffer);

/**
 * Exports the contents of the buffer to an html file 
 *
 * @param buffer The contents of the buffer
 * @param path The path to export the html file to
 */
WEBCAPI void Export(char* buffer, Cstr path);

/**
 * TODO
 */
WEBCAPI void ExportRoute(Route route);

// ############ Attributes ############ //

/**
 * Creates an Attribute struct pointer
 *
 * @param name AttributeName enum to specify the attribute name
 * @param value The value of the attribute
 *
 * @return Attribute*
 */
WEBCAPI Attribute* MakeAttribute(AttributeName name, Cstr value);

/**
 * Creates an Attribute struct pointer array
 *
 * @param first The first attribute
 * @param ... The rest of the attributes
 *
 * @return Attribute** The array
 */
WEBCAPI Attribute** MakeAttributeList(Attribute* first, ...);

/**
 * Stringifies the AttributeName enum
 *
 * @param attr The enum
 *
 * @return Cstr
 */
WEBCAPI Cstr AttributeNameToString(AttributeName attr);


// ############ Elements ############ //

/**
 * TODO
 */
WEBCAPI void Head(char** buffer, Cstr title, Tag* first, ...);

/* TODO */

WEBCAPI void HtmlStart(char** buffer, Cstr lang);
WEBCAPI void HtmlEnd(char** buffer);
WEBCAPI void BodyEnd(char** buffer);
WEBCAPI void BodyStart(char** buffer);
WEBCAPI void ScriptStart(char** buffer);
WEBCAPI void ScriptEnd(char** buffer);
WEBCAPI void StyleStart(char** buffer);
WEBCAPI void StyleEnd(char** buffer);

/* TODO */

// Helpers
WEBCAPI void Block(char** buffer, Tag* tag, BlockContents contents);
WEBCAPI void BlockAttr(char** buffer, Cstr name, Attribute** attributes, BlockContents contents);
WEBCAPI void InlineBlock(char** buffer, Cstr name, Attribute** attributes, Cstr text);

/* TODO */

WEBCAPI void Abbr(char** buffer, Attribute** attributes, Cstr abbr);
WEBCAPI void Address(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Anchor(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void AnchorEx(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Blockquote(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Bold(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Cite(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Code(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Del(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Div(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Heading(char** buffer, Attribute** attributes, size_t size, Cstr text);
WEBCAPI void Paragraph(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void ParagraphEx(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Li(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void LiEx(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Ul(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Ol(char** buffer, Attribute** attributes, BlockContents contents);

/* TODO */

#define PlainText(buffer, text) \
    Append(buffer, text)
#define Javascript(buffer, text) \
    PlainText(buffer, text)
#define Input(buffer, attr, ...) \
    Append(buffer, TagToString(MakeTag("input", attr, ##__VA_ARGS__)))
#define Img(buffer, attr, ...) \
    Append(buffer, TagToString(MakeTag("img", attr, ##__VA_ARGS__)))
#define Link(buffer, attr, ...) \
    Append(buffer, TagToString(MakeTag("link", attr, ##__VA_ARGS__)))
#define Br(buffer) \
    Append(buffer, TagToString(MakeTag("br", NULL)))
#define Hr(buffer) \
    Append(buffer, TagToString(MakeTag("hr", NULL)))


#endif // WEBC_H
