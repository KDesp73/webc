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
#define VERSION_PATCH 11
#define VERSION "0.0.11"

#ifndef WEBCAPI
    #define WEBCAPI extern // Functions defined as 'extern' by default (implicit specifiers)
#endif

// ############ Includes  ############ //

#define CLIB_IMPLEMENTATION
#include "extern/clib.h"

#ifdef __cplusplus
extern "C" {
#endif

// ############ Types ############ //

typedef enum {
    ATTR_ACTION = 0,
    ATTR_ALT,
    ATTR_CHARSET,
    ATTR_CLASS,
    ATTR_CONTENT,
    ATTR_HEIGHT,
    ATTR_HREF,
    ATTR_HTTP_EQUIV,
    ATTR_ID,
    ATTR_METHOD,
    ATTR_NAME,
    ATTR_PROPERTY,
    ATTR_REL,
    ATTR_SRC,
    ATTR_STYLE,
    ATTR_TABINDEX,
    ATTR_TARGET,
    ATTR_TITLE,
    ATTR_TYPE,
    ATTR_VALUE,
    ATTR_CHECKED,
    ATTR_WIDTH,
    ATTR_ROLE,
    ATTR_FOR,
    ATTR_ARIA_LABEL,
    ATTR_PLACEHOLDER,
    ATTR_DATA_PREFIX,
    ATTR_DATA_TIP,
    ATTR_MIN,
    ATTR_MAX,
    ATTR_STEP,
    ATTR_DISABLED,
    ATTR_SELECTED,
    ATTR_ONCLICK,
    ATTR_ONCONTEXTMENU,
    ATTR_ONDBLCLICK,
    ATTR_ONMOUSEDOWN,
    ATTR_ONMOUSEENTER,
    ATTR_ONMOUSELEAVE,
    ATTR_ONMOUSEMOVE,
    ATTR_ONMOUSEOUT,
    ATTR_ONMOUSEOVER,
    ATTR_ONMOUSEUP,
    
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
    Cstr onclick;
    Cstr oncontextmenu;
    Cstr ondblclick;
    Cstr onmousedown;
    Cstr onmouseenter;
    Cstr onmouseleave;
    Cstr onmousemove;
    Cstr onmouseout;
    Cstr onmouseover;
    Cstr onmouseup;
} MouseEvents;

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
    Cstr tabindex;
    Cstr value;
    Cstr for_;
    Cstr aria_label;
    Cstr name;
    Cstr role;
    Cstr min;
    Cstr max;
    Cstr step;
    Cstr checked;
    int disabled;
    int selected;
    Cstr placeholder;
    Cstr data_prefix;
    Cstr data_tip;
    MouseEvents mouse_events;
} Modifier;


#define CLASS(cls) \
    WEBC_UseModifier((Modifier) { .class = cls }) 

#define STYLE(stl) \
    WEBC_UseModifier((Modifier) { .style= stl }) 

#define SRC(s) \
    WEBC_UseModifier((Modifier) { .src= s }) 


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
 * Allocates minimal memory for the buffer and cleans the allocated memory space
 *
 * @return char* The allocated buffer
 */
WEBCAPI char* WEBC_BufferInit();

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

#define META_AUTHOR_TAG(author) \
    WEBC_MakeTag(META, \
        WEBC_MakeAttributeList( \
            WEBC_MakeAttribute(ATTR_NAME, "author"), \
            WEBC_MakeAttribute(ATTR_CONTENT, author), \
            NULL \
        ) \
    )

#define META_DESCRIPTION_TAG(description) \
    WEBC_MakeTag(META, \
        WEBC_MakeAttributeList( \
            WEBC_MakeAttribute(ATTR_NAME, "description"), \
            WEBC_MakeAttribute(ATTR_CONTENT, description), \
            NULL \
        ) \
    )

#define META_KEYWORDS_TAG(keywords) \
    WEBC_MakeTag(META, \
        WEBC_MakeAttributeList( \
            WEBC_MakeAttribute(ATTR_NAME, "keywords"), \
            WEBC_MakeAttribute(ATTR_CONTENT, keywords), \
            NULL \
        ) \
    )

#define LINK_STYLESHEET_TAG(file) \
    WEBC_MakeTag(LINK, \
        WEBC_MakeAttributeList( \
            WEBC_MakeAttribute(ATTR_REL, "stylesheet"), \
            WEBC_MakeAttribute(ATTR_HREF, file), \
            NULL \
        ) \
    )

#define LINK_ICON_TAG(file) \
    WEBC_MakeTag(LINK, \
        WEBC_MakeAttributeList( \
            WEBC_MakeAttribute(ATTR_REL, "icon"), \
            WEBC_MakeAttribute(ATTR_HREF, file), \
            WEBC_MakeAttribute(ATTR_TYPE, "image/x-icon"), \
            NULL \
        ) \
    )

#define BASE_TAG(prefix) \
    WEBC_MakeTag(BASE, \
        WEBC_MakeAttributeList( \
            WEBC_MakeAttribute(ATTR_HREF, prefix), \
            NULL \
        ) \
    )

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
WEBCAPI void WEBC_TagStart(char** buffer, Cstr tag_name, AttributeList attributes);
WEBCAPI void WEBC_TagEnd(char** buffer, Cstr tag_name);
WEBCAPI void WEBC_AbbrStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_AbbrEnd(char** buffer);
WEBCAPI void WEBC_AddressStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_AddressEnd(char** buffer);
WEBCAPI void WEBC_AnchorStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_AnchorEnd(char** buffer);
WEBCAPI void WEBC_BodyStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_BodyEnd(char** buffer);
WEBCAPI void WEBC_AreaStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_AreaEnd(char** buffer);
WEBCAPI void WEBC_ArticleStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_ArticleEnd(char** buffer);
WEBCAPI void WEBC_AsideStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_AsideEnd(char** buffer);
WEBCAPI void WEBC_AudioStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_AudioEnd(char** buffer);
WEBCAPI void WEBC_BStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_BEnd(char** buffer);
WEBCAPI void WEBC_BdiStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_BdiEnd(char** buffer);
WEBCAPI void WEBC_BdoStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_BdoEnd(char** buffer);
WEBCAPI void WEBC_BlockquoteStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_BlockquoteEnd(char** buffer);
WEBCAPI void WEBC_BoldStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_BoldEnd(char** buffer);
WEBCAPI void WEBC_BrStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_BrEnd(char** buffer);
WEBCAPI void WEBC_ButtonStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_ButtonEnd(char** buffer);
WEBCAPI void WEBC_CanvasStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_CanvasEnd(char** buffer);
WEBCAPI void WEBC_CaptionStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_CaptionEnd(char** buffer);
WEBCAPI void WEBC_CiteStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_CiteEnd(char** buffer);
WEBCAPI void WEBC_CodeStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_CodeEnd(char** buffer);
WEBCAPI void WEBC_ColStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_ColEnd(char** buffer);
WEBCAPI void WEBC_ColgroupStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_ColgroupEnd(char** buffer);
WEBCAPI void WEBC_DataStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_DataEnd(char** buffer);
WEBCAPI void WEBC_DatalistStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_DatalistEnd(char** buffer);
WEBCAPI void WEBC_DdStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_DdEnd(char** buffer);
WEBCAPI void WEBC_DelStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_HtmlStart(char**buffer, Cstr lang);
WEBCAPI void WEBC_HtmlEnd(char**buffer);
WEBCAPI void WEBC_DelEnd(char** buffer);
WEBCAPI void WEBC_DetailsStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_DetailsEnd(char** buffer);
WEBCAPI void WEBC_DfnStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_DfnEnd(char** buffer);
WEBCAPI void WEBC_DialogStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_DialogEnd(char** buffer);
WEBCAPI void WEBC_DivStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_DivEnd(char** buffer);
WEBCAPI void WEBC_DlStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_DlEnd(char** buffer);
WEBCAPI void WEBC_DtStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_DtEnd(char** buffer);
WEBCAPI void WEBC_EmStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_EmEnd(char** buffer);
WEBCAPI void WEBC_EmbedStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_EmbedEnd(char** buffer);
WEBCAPI void WEBC_FieldsetStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_FieldsetEnd(char** buffer);
WEBCAPI void WEBC_FigcaptionStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_FigcaptionEnd(char** buffer);
WEBCAPI void WEBC_FigureStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_FigureEnd(char** buffer);
WEBCAPI void WEBC_FooterStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_FooterEnd(char** buffer);
WEBCAPI void WEBC_FormStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_FormEnd(char** buffer);
WEBCAPI void WEBC_HeaderStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_HeaderEnd(char** buffer);
WEBCAPI void WEBC_HeadingStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_HeadingEnd(char** buffer);
WEBCAPI void WEBC_HgroupStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_HgroupEnd(char** buffer);
WEBCAPI void WEBC_HrStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_HrEnd(char** buffer);
WEBCAPI void WEBC_IStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_IEnd(char** buffer);
WEBCAPI void WEBC_IframeStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_IframeEnd(char** buffer);
WEBCAPI void WEBC_ImgStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_ImgEnd(char** buffer);
WEBCAPI void WEBC_InputStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_InputEnd(char** buffer);
WEBCAPI void WEBC_InsStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_InsEnd(char** buffer);
WEBCAPI void WEBC_KbdStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_KbdEnd(char** buffer);
WEBCAPI void WEBC_LabelStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_LabelEnd(char** buffer);
WEBCAPI void WEBC_LegendStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_LegendEnd(char** buffer);
WEBCAPI void WEBC_LiStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_LiEnd(char** buffer);
WEBCAPI void WEBC_LinkStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_LinkEnd(char** buffer);
WEBCAPI void WEBC_MainStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_MainEnd(char** buffer);
WEBCAPI void WEBC_MapStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_MapEnd(char** buffer);
WEBCAPI void WEBC_MarkStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_MarkEnd(char** buffer);
WEBCAPI void WEBC_MenuStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_MenuEnd(char** buffer);
WEBCAPI void WEBC_MeterStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_MeterEnd(char** buffer);
WEBCAPI void WEBC_NavStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_NavEnd(char** buffer);
WEBCAPI void WEBC_NoscriptStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_NoscriptEnd(char** buffer);
WEBCAPI void WEBC_ObjectStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_ObjectEnd(char** buffer);
WEBCAPI void WEBC_OlStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_OlEnd(char** buffer);
WEBCAPI void WEBC_OptgroupStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_OptgroupEnd(char** buffer);
WEBCAPI void WEBC_OptionStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_OptionEnd(char** buffer);
WEBCAPI void WEBC_OutputStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_OutputEnd(char** buffer);
WEBCAPI void WEBC_ParagraphStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_ParagraphEnd(char** buffer);
WEBCAPI void WEBC_ParamStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_ParamEnd(char** buffer);
WEBCAPI void WEBC_PictureStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_PictureEnd(char** buffer);
WEBCAPI void WEBC_PreStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_PreEnd(char** buffer);
WEBCAPI void WEBC_ProgressStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_ProgressEnd(char** buffer);
WEBCAPI void WEBC_QStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_QEnd(char** buffer);
WEBCAPI void WEBC_RpStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_RpEnd(char** buffer);
WEBCAPI void WEBC_RtStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_RtEnd(char** buffer);
WEBCAPI void WEBC_RubyStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_RubyEnd(char** buffer);
WEBCAPI void WEBC_SStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_SEnd(char** buffer);
WEBCAPI void WEBC_SampStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_SampEnd(char** buffer);
WEBCAPI void WEBC_ScriptStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_ScriptEnd(char** buffer);
WEBCAPI void WEBC_SearchStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_SearchEnd(char** buffer);
WEBCAPI void WEBC_SectionStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_SectionEnd(char** buffer);
WEBCAPI void WEBC_SelectStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_SelectEnd(char** buffer);
WEBCAPI void WEBC_SmallStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_SmallEnd(char** buffer);
WEBCAPI void WEBC_SourceStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_SourceEnd(char** buffer);
WEBCAPI void WEBC_SpanStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_SpanEnd(char** buffer);
WEBCAPI void WEBC_StrongStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_StrongEnd(char** buffer);
WEBCAPI void WEBC_StyleStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_StyleEnd(char** buffer);
WEBCAPI void WEBC_SubStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_SubEnd(char** buffer);
WEBCAPI void WEBC_SummaryStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_SummaryEnd(char** buffer);
WEBCAPI void WEBC_SupStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_SupEnd(char** buffer);
WEBCAPI void WEBC_SvgStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_SvgEnd(char** buffer);
WEBCAPI void WEBC_TableStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_TableEnd(char** buffer);
WEBCAPI void WEBC_TbodyStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_TbodyEnd(char** buffer);
WEBCAPI void WEBC_TdStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_TdEnd(char** buffer);
WEBCAPI void WEBC_TemplateStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_TemplateEnd(char** buffer);
WEBCAPI void WEBC_TextareaStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_TextareaEnd(char** buffer);
WEBCAPI void WEBC_TfootStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_TfootEnd(char** buffer);
WEBCAPI void WEBC_ThStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_ThEnd(char** buffer);
WEBCAPI void WEBC_TheadStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_TheadEnd(char** buffer);
WEBCAPI void WEBC_TimeStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_TimeEnd(char** buffer);
WEBCAPI void WEBC_TrStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_TrEnd(char** buffer);
WEBCAPI void WEBC_TrackStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_TrackEnd(char** buffer);
WEBCAPI void WEBC_UStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_UEnd(char** buffer);
WEBCAPI void WEBC_UlStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_UlEnd(char** buffer);
WEBCAPI void WEBC_VarStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_VarEnd(char** buffer);
WEBCAPI void WEBC_VideoStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_VideoEnd(char** buffer);
WEBCAPI void WEBC_WbrStart(char** buffer, AttributeList attributes);
WEBCAPI void WEBC_WbrEnd(char** buffer);

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
The following macros are used to append the html tags that don't use tags
*/

#define WEBC_PlainText(buffer, text) \
    WEBC_AppendLn(buffer, text)
#define WEBC_Javascript(buffer, text) \
    WEBC_PlainText(buffer, text)
#define WEBC_Comment(buffer, text) \
    do { \
        char* comment = clib_format_text("<!--%s-->\n", text); \
        WEBC_AppendLn(buffer, comment); \
        free(comment); \
    } while(0);
#define WEBC_LogComment(buffer, tag, text) \
    do { \
        char* comment = clib_format_text("<!--[%s] %s-->\n", tag, text); \
        WEBC_AppendLn(buffer, comment); \
        free(comment); \
    } while(0);
#define WEBC_InfoComment(buffer, text) \
    WEBC_LogComment(buffer, "INFO", text);
#define WEBC_TipComment(buffer, text) \
    WEBC_LogComment(buffer, "TIP", text);
#define WEBC_TodoComment(buffer, text) \
    WEBC_LogComment(buffer, "TODO", text);


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

#ifdef __cplusplus
}
#endif

#endif // WEBC_H
