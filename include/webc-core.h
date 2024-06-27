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
#define VERSION_PATCH 5
#define VERSION "0.0.5"

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
WEBCAPI Tag* WEBC_MakeTag(Cstr name, AttributeList attributes);

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
WEBCAPI Tag* WEBC_MakeTagAttr(Cstr name, Attribute* first, ...);

/**
 * Creates a closing tag (</>) for any Tag struct pointer
 *
 * @param tag The Tag struct pointer
 *
 * @return Cstr
 */
WEBCAPI Cstr WEBC_ClosingTag(Tag* tag);

/**
 * Creates the opening tag (<name attr1="value" attr2="value">) for any Tag struct pointer
 *
 * @param tag The Tag struct pointer
 *
 * @return Cstr
 */
WEBCAPI Cstr WEBC_TagToString(Tag* tag);

/**
 * Frees all the allocated memory of the Tag struct pointer
 *
 * @param tag The tag pointer's pointer
 */
WEBCAPI void WEBC_CleanTag(Tag** tag);

#define WEBC_OPENING_TAG(name) clib_format_text("<%s>", name)
#define WEBC_CLOSING_TAG(name) clib_format_text("</%s>", name)

// ############ Utils ############ //

/**
 * Appends text (and a newline) to the buffer
 *
 * @param buffer The buffer's pointer
 * @param text The text to append
 */
WEBCAPI void WEBC_AppendLn(char** buffer, Cstr text);

/**
 * Appends text (no newline) to the buffer
 *
 * @param buffer The buffer's pointer
 * @param text The text to append
 */
WEBCAPI void WEBC_Append(char** buffer, Cstr text);

/**
 * Frees the buffer's pointer
 *
 * @param buffer The buffer to free
 */
WEBCAPI void WEBC_Clean(char** buffer);

/**
 * Appends the file contents to the buffer
 *
 * @param buffer The buffer to append the contents
 * @path The file path
 */
WEBCAPI void WEBC_IntegrateFile(char** buffer, Cstr path);

// ############ Attributes ############ //

/**
 * Creates an Attribute struct pointer
 *
 * @param name AttributeName enum to specify the attribute name
 * @param value The value of the attribute
 *
 * @return Attribute*
 */
WEBCAPI Attribute* WEBC_MakeAttribute(AttributeName name, Cstr value);

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
WEBCAPI AttributeList WEBC_MakeAttributeList(Attribute* first, ...);

/**
 * Frees the memory allocated bu the AttributeList
 *
 * @param list The AttributeList
 */
WEBCAPI void WEBC_CleanAttributeList(AttributeList list);

/**
 * Stringifies the AttributeName enum
 *
 * @param attr The enum
 *
 * @return Cstr
 */
WEBCAPI Cstr WEBC_AttributeNameToString(AttributeName attr);

/**
 * Converts the Modifier struct to an AttributeList to be used on any element
 *
 * @param modifier The Modifier struct
 *
 * @return AttrubuteList
 */
WEBCAPI AttributeList WEBC_UseModifier(Modifier modifier);

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
WEBCAPI void WEBC_Head(char** buffer, Cstr title, Tag* first, ...);

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
WEBCAPI void WEBC_HtmlStart(char** buffer, Cstr lang); 
WEBCAPI void WEBC_HtmlEnd(char** buffer);
WEBCAPI void WEBC_BodyEnd(char** buffer);
WEBCAPI void WEBC_BodyStart(char** buffer);
WEBCAPI void WEBC_ScriptStart(char** buffer);
WEBCAPI void WEBC_ScriptEnd(char** buffer);
WEBCAPI void WEBC_StyleStart(char** buffer);
WEBCAPI void WEBC_StyleEnd(char** buffer);
WEBCAPI void WEBC_DivStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_DivEnd(char** buffer);
WEBCAPI void WEBC_MainStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_MainEnd(char** buffer);
WEBCAPI void WEBC_UlStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_UlEnd(char** buffer);
WEBCAPI void WEBC_FooterStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_FooterEnd(char** buffer);

// TODO: maybe add more Start-End pair for easier use


// Helpers
WEBCAPI void WEBC_Block(char** buffer, Tag* tag, BlockContents contents);
WEBCAPI void WEBC_BlockAttr(char** buffer, Cstr name, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_InlineBlock(char** buffer, Cstr name, AttributeList attributes, Cstr text);

/*
The following methods are used to append all the available (but not deprecated) html tags to the buffer
*/

/**/

WEBCAPI void WEBC_Abbr(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Address(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Anchor(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_AnchorBlock(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Area(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Article(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Aside(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Audio(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_B(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Bdi(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Bdo(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Blockquote(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Bold(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Br(char** buffer);
WEBCAPI void WEBC_Button(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_ButtonBlock(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Canvas(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Caption(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Cite(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Code(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Col(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Colgroup(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Data(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Datalist(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Dd(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Del(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Del(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Details(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Dfn(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Dialog(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Div(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Dl(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Dt(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Em(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Embed(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Fieldset(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Figcaption(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Figure(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Footer(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Form (char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Header(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Heading(char** buffer, AttributeList attributes, size_t size, Cstr text);
WEBCAPI void WEBC_Hgroup(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Hr(char** buffer);
WEBCAPI void WEBC_I(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Iframe(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Img(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_Input(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_Ins(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Kbd(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Label(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Legend(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Li(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_LiBlock(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Link(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_Main(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Map(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Mark(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Menu(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Meter(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Nav(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Noscript(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Object(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Ol(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Optgroup(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Option(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Output(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Paragraph(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_ParagraphBlock(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Param(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Picture(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Pre(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Progress(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Q(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Rp(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Rt(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Ruby(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_S(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Samp(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Script(char**buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Search(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Section(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Select(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Small(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Source(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_Span(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Strong(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Style(char**buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Sub(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Summary(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Sup(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Svg(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Table(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Tbody(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Td(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Template(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Textarea(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Tfoot(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Th(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Thead(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Time(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Tr(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Track(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_U(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Ul(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Var(char** buffer, AttributeList attributes, Cstr text);
WEBCAPI void WEBC_Video(char** buffer, AttributeList attributes, BlockContents contents);
WEBCAPI void WEBC_Wbr(char** buffer, AttributeList attributes, Cstr text);

/* 
The following macros are used to append the html tags that don't require closing to the buffer
*/

#define WEBC_PlainText(buffer, text) \
    WEBC_AppendLn(buffer, text)
#define WEBC_Javascript(buffer, text) \
    WEBC_PlainText(buffer, text)

/*
Specific Heading elements
*/

#define WEBC_H1(buffer, attr, text) \
    WEBC_Heading(buffer, attr, 1, text);
#define WEBC_H2(buffer, attr, text) \
    WEBC_Heading(buffer, attr, 2, text);
#define WEBC_H3(buffer, attr, text) \
    WEBC_Heading(buffer, attr, 3, text);
#define WEBC_H4(buffer, attr, text) \
    WEBC_Heading(buffer, attr, 4, text);
#define WEBC_H5(buffer, attr, text) \
    WEBC_Heading(buffer, attr, 5, text);
#define WEBC_H6(buffer, attr, text) \
    WEBC_Heading(buffer, attr, 6, text);

#endif // WEBC_H
