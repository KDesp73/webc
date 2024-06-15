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
#define VERSION_PATCH 2
#define VERSION "0.0.2"

#ifndef WEBCAPI
    #define WEBCAPI extern // Functions defined as 'extern' by default (implicit specifiers)
#endif

// ############ Includes  ############ //

#define CLIB_IMPLEMENTATION
#include "extern/clib.h"

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
 * Exports the contents of the buffer to an html file, while also creating the path that is being passed
 *
 * @param buffer The contents of the buffer
 * @param path The path to export the html file to
 */
WEBCAPI void Export(char* buffer, Cstr path);

/**
 * Exports the Route struct containing a buffer and a path
 *
 * @param route The Route struct
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

#define META "meta"
#define LINK "link"
#define BASE "base"

/**
 * Appends the <head> block to the buffer with the default meta tags included
 *
 * @param buffer The buffer to append to
 * @param title The title of the web page
 * @param first The first tag to be contained in the head block
 * @param ... The rest of the tags
 */
WEBCAPI void Head(char** buffer, Cstr title, Tag* first, ...);

/* 
The following methods define the major blocks of an HTML document by appending them to the buffer.
Start and End reference the opening and closing tags.
*/

/**
 * Appends the html opening tag to the buffer (<html lang="lang">)
 *
 * @param buffer The buffer to append to
 * @param lang The webpage's language
 */
WEBCAPI void HtmlStart(char** buffer, Cstr lang); 
WEBCAPI void HtmlEnd(char** buffer);
WEBCAPI void BodyEnd(char** buffer);
WEBCAPI void BodyStart(char** buffer);
WEBCAPI void ScriptStart(char** buffer);
WEBCAPI void ScriptEnd(char** buffer);
WEBCAPI void StyleStart(char** buffer);
WEBCAPI void StyleEnd(char** buffer);


// Helpers
WEBCAPI void Block(char** buffer, Tag* tag, BlockContents contents);
WEBCAPI void BlockAttr(char** buffer, Cstr name, Attribute** attributes, BlockContents contents);
WEBCAPI void InlineBlock(char** buffer, Cstr name, Attribute** attributes, Cstr text);

/*
The following methods are used to append all the available (but not deprecated) html tags to the buffer
*/

/**/
WEBCAPI void Abbr(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Address(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Anchor(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void AnchorBlock(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Area(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Article(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Aside(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Audio(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void B(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Bdi(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Bdo(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Blockquote(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Bold(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Button(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void ButtonBlock(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Canvas(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Caption(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Cite(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Code(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Col(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Colgroup(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Data(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Datalist(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Dd(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Del(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Del(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Details(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Dfn(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Dialog(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Div(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Dl(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Dt(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Em(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Embed(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Fieldset(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Figcaption(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Figure(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Footer(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Form (char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Heading(char** buffer, Attribute** attributes, size_t size, Cstr text);
WEBCAPI void Hgroup(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void I(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Iframe(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Ins(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Kbd(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Label(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Legend(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Li(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void LiBlock(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Main(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Map(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Mark(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Menu(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Meter(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Nav(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Noscript(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Object(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Ol(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Optgroup(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Option(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Output(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Paragraph(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void ParagraphBlock(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Param(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Picture(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Pre(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Progress(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Q(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Rp(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Rt(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Ruby(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void S(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Samp(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Search(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Section(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Select(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Small(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Span(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Strong(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Sub(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Summary(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Sup(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Svg(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Table(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Tbody(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Td(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Template(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Textarea(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Tfoot(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Th(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Thead(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Time(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Tr(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void U(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Ul(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Var(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void Video(char** buffer, Attribute** attributes, BlockContents contents);
WEBCAPI void Wbr(char** buffer, Attribute** attributes, Cstr text);

/* 
The following macros are used to append the html tags that don't require closing to the buffer
*/

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
#define Source(buffer, attr, ...) \
    Append(buffer, TagToString(MakeTag("source", attr, ##__VA_ARGS__)))
#define Track(buffer, attr, ...) \
    Append(buffer, TagToString(MakeTag("track", attr, ##__VA_ARGS__)))


#endif // WEBC_H
