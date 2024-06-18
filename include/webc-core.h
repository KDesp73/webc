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
 */

/**
 * @file webc-core.h
 * @author KDesp73 (Konstantinos Despoinidis)
 */

#ifndef WEBC_H
#define WEBC_H

#define VERSION_MAJOR 0
#define VERSION_MINOR 0
#define VERSION_PATCH 4
#define VERSION "0.0.4"

#ifndef WEBCAPI
    #define WEBCAPI extern // Functions defined as 'extern' by default (implicit specifiers)
#endif

// ############ Includes  ############ //

#define CLIB_IMPLEMENTATION
#include "extern/clib.h"

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
    ATTR_ID,
    ATTR_CLASS,
    ATTR_METHOD,
    ATTR_TYPE,
    ATTR_ACTION,
    ATTR_VALUE,
    
    ATTRIBUTE_NAME_COUNT
} AttributeName;

typedef struct {
    AttributeName name; 
    char* value;
} Attribute;

#define NO_ATTRIBUTES (AttributeList){0}

typedef struct {
    Attribute** items;
    size_t count;
} AttributeList;

typedef struct {
    Cstr name;
    AttributeList attributes;
} Tag;

typedef struct {
    Cstr style;
    Cstr src;
    Cstr alt;
    Cstr href;
    Cstr target;
    size_t width;
    size_t height;
    Cstr class;
    Cstr id;
    Cstr method;
    Cstr action;
    Cstr type;
    Cstr value;
} Modifier;

typedef void (*BlockContents)(char** buffer);

// ############ Tags ############ //

/**
 * Creates a Tag struct pointer
 *
 * @param name The tage name
 * @param attributes The attribute struct containing the attributes
 *
 * @return Tag*
 */
WEBCAPI Tag* MakeTag(Cstr name, AttributeList attributes);

/**
 * Creates a Tag struct pointer
 *
 * WARNING: NULL terminate the arguments list
 *
 * @param name The tage name
 * @param first The first Attribute struct pointer for the tag
 * @param ... The rest of the Attribute struct pointers
 *
 * @return Tag*
 */
WEBCAPI Tag* MakeTagAttr(Cstr name, Attribute* first, ...);

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

/**
 * Frees all the allocated memory of the Tag struct pointer
 *
 * @param tag The tag pointer's pointer
 */
WEBCAPI void CleanTag(Tag** tag);

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
 * Appends the file contents to the buffer
 *
 * @param buffer The buffer to append the contents
 * @path The file path
 */
WEBCAPI void IntegrateFile(char** buffer, Cstr path);

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
 * WARNING: NULL terminate the arguments list
 *
 * @param first The first attribute
 * @param ... The rest of the attributes
 *
 * @return AttributeList The array
 */
WEBCAPI AttributeList MakeAttributeList(Attribute* first, ...);

/**
 * Stringifies the AttributeName enum
 *
 * @param attr The enum
 *
 * @return Cstr
 */
WEBCAPI Cstr AttributeNameToString(AttributeName attr);

/**
 * Converts the Modifier struct to an AttributeList to be used on any element
 *
 * @param modifier The Modifier struct
 *
 * @return AttrubuteList
 */
WEBCAPI AttributeList UseModifier(Modifier modifier);

// ############ Elements ############ //

#define META "meta"
#define LINK "link"
#define BASE "base"

/**
 * Appends the <head> block to the buffer with the default meta tags included
 *
 * WARNING: NULL terminate the arguments list
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
WEBCAPI void DivStart(char** buffer);
WEBCAPI void DivEnd(char** buffer);
// TODO: maybe add more Start-End pair for easier use


// Helpers
WEBCAPI void Block(char** buffer, Tag* tag, BlockContents contents);
WEBCAPI void BlockAttr(char** buffer, Cstr name, AttributeList attributes, BlockContents contents);
WEBCAPI void InlineBlock(char** buffer, Cstr name, AttributeList attributes, Cstr text);

/*
The following methods are used to append all the available (but not deprecated) html tags to the buffer
*/

/**/

WEBCAPI void Abbr(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Address(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Anchor(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void AnchorBlock(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Area(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Article(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Aside(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Audio(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void B(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Bdi(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Bdo(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Blockquote(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Bold(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Button(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void ButtonBlock(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Canvas(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Caption(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Cite(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Code(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Col(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Colgroup(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Data(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Datalist(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Dd(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Del(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Del(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Details(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Dfn(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Dialog(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Div(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Dl(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Dt(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Em(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Embed(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Fieldset(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Figcaption(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Figure(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Footer(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Form (char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Header(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Heading(char** buffer, AttributeList attributes, size_t size, Cstr text);
WEBCAPI void Hgroup(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void I(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Iframe(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Ins(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Kbd(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Label(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Legend(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Li(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void LiBlock(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Main(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Map(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Mark(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Menu(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Meter(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Nav(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Noscript(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Object(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Ol(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Optgroup(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Option(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Output(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Paragraph(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void ParagraphBlock(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Param(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Picture(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Pre(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Progress(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Q(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Rp(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Rt(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Ruby(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void S(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Samp(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Script(char**buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Search(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Section(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Select(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Small(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Span(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Strong(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Style(char**buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Sub(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Summary(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Sup(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Svg(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Table(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Tbody(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Td(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Template(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Textarea(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Tfoot(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Th(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Thead(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Time(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Tr(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void U(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Ul(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Var(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void Video(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void Wbr(char** buffer, AttributeList attributes, Cstr text);

/* 
The following macros are used to append the html tags that don't require closing to the buffer
*/

#define PlainText(buffer, text) \
    Append(buffer, text)
#define Javascript(buffer, text) \
    PlainText(buffer, text)
#define Input(buffer, attr) \
    Append(buffer, TagToString(MakeTag("input", attr)))
#define Img(buffer, attr) \
    Append(buffer, TagToString(MakeTag("img", attr)))
#define Link(buffer, attr) \
    Append(buffer, TagToString(MakeTag("link", attr)))
#define Br(buffer) \
    Append(buffer, TagToString(MakeTag("br", NO_ATTRIBUTES)))
#define Hr(buffer) \
    Append(buffer, TagToString(MakeTag("hr", NO_ATTRIBUTES)))
#define Source(buffer, attr) \
    Append(buffer, TagToString(MakeTag("source", attr)))
#define Track(buffer, attr) \
    Append(buffer, TagToString(MakeTag("track", attr)))

/*
Specific Heading elements
*/

#define H1(buffer, attr, text) \
    Heading(buffer, attr, 1, text);
#define H2(buffer, attr, text) \
    Heading(buffer, attr, 2, text);
#define H3(buffer, attr, text) \
    Heading(buffer, attr, 3, text);
#define H4(buffer, attr, text) \
    Heading(buffer, attr, 4, text);
#define H5(buffer, attr, text) \
    Heading(buffer, attr, 5, text);
#define H6(buffer, attr, text) \
    Heading(buffer, attr, 6, text);

#endif // WEBC_H
