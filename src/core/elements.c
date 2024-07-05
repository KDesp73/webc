#include "webc-core.h"
#include <assert.h>
#include <stdlib.h>

WEBCAPI void WEBC_BlockAttr(char** buffer, Cstr name, AttributeList attributes, BlockContents contents)
{
    Tag* tag = WEBC_MakeTag(name, attributes);
    WEBC_Block(buffer, tag, contents);
    WEBC_CleanTag(&tag);
}

WEBCAPI void WEBC_Block(char** buffer, Tag* tag, BlockContents contents)
{
    assert(tag != NULL);

    char* opening = (char*) WEBC_TagToString(tag);
    char* closing = (char*) WEBC_ClosingTag(tag);
    WEBC_AppendLn(buffer, opening);
    contents(buffer);
    WEBC_AppendLn(buffer, closing);

    free(opening);
    free(closing);
}

WEBCAPI void WEBC_InlineBlock(char** buffer, Cstr name, AttributeList attributes, Cstr text)
{

    Tag* tag = WEBC_MakeTag(name, attributes);
    char* opening = (char*) WEBC_TagToString(tag);
    char* closing = (char*) WEBC_ClosingTag(tag);
    char* tag_str = clib_format_text("%s%s%s", opening, text, closing);

    WEBC_AppendLn(buffer, tag_str);
    free(opening);
    free(closing);
    free(tag_str);
    WEBC_CleanTag(&tag);
}

/* ######################### Elements Below ######################### */

WEBCAPI void WEBC_Head(char** buffer, Cstr title, Tag* first, ...)
{
    char* head = (char*) malloc(1);
    head[0] = '\0';
    WEBC_AppendLn(&head, "<head>\n<meta charset=\"UTF-8\">\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");

    if (first != NULL) {
        char* first_tag_str = (char*) WEBC_TagToString(first);
        WEBC_AppendLn(&head, first_tag_str);
        free(first_tag_str);
        WEBC_CleanTag(&first);

        va_list args;
        va_start(args, first);
        Tag* tag = va_arg(args, Tag*);
        while (tag != NULL) {
            char* tag_str = (char*) WEBC_TagToString(tag);
            WEBC_AppendLn(&head, tag_str);
            free(tag_str);
            WEBC_CleanTag(&tag);
            tag = va_arg(args, Tag*);
        }
        va_end(args);
    }

    Cstr title_tag = clib_format_text("<title>%s</title>", title);
    Cstr header_str = clib_format_text("%s\n%s\n</head>", head, title_tag);
    WEBC_AppendLn(buffer, header_str);

    free((char*) title_tag);
    free((char*) header_str);
    free(head);
}

WEBCAPI void WEBC_HtmlStart(char**buffer, Cstr lang)
{
    if(lang == NULL){
        PANIC("Language is NULL");
    }

    *buffer = (char*) malloc(1);
    if(*buffer == NULL){
        PANIC("Couldnt initialize buffer");
    }
    *buffer[0] = '\0';

    char* temp = clib_format_text("<!DOCTYPE html>\n<html lang=\"%s\">\n", lang);

    *buffer = (char*) realloc(*buffer, strlen(temp) + 1);
    if(*buffer == NULL){
        PANIC("Couldnt reallocate buffer");
    }

    strcpy(*buffer, temp);
    free(temp);
}

WEBCAPI void WEBC_HtmlEnd(char** buffer)
{
    char* tag = WEBC_CLOSING_TAG("html");
    WEBC_AppendLn(buffer, tag);
    free(tag);
}


WEBCAPI void WEBC_BodyStart(char** buffer)
{
    char* tag = WEBC_OPENING_TAG("body");
    WEBC_AppendLn(buffer, tag);
    free(tag);
}

WEBCAPI void WEBC_BodyEnd(char** buffer)
{
    char* tag = WEBC_CLOSING_TAG("body");
    WEBC_AppendLn(buffer, tag);
    free(tag);
}

WEBCAPI void WEBC_ScriptStart(char** buffer)
{
    char* tag = WEBC_OPENING_TAG("script");
    WEBC_AppendLn(buffer, tag);
    free(tag);
}

WEBCAPI void WEBC_ScriptEnd(char** buffer)
{
    char* tag = WEBC_CLOSING_TAG("script");
    WEBC_AppendLn(buffer, tag);
    free(tag);
}

WEBCAPI void WEBC_StyleStart(char** buffer)
{
    char* tag = WEBC_OPENING_TAG("style");
    WEBC_AppendLn(buffer, tag);
    free(tag);
}

WEBCAPI void WEBC_StyleEnd(char** buffer)
{
    char* tag = WEBC_CLOSING_TAG("style");
    WEBC_AppendLn(buffer, tag);
    free(tag);
}

WEBCAPI void WEBC_DivStart(char** buffer, AttributeList attributes)
{
    Tag* tag = WEBC_MakeTag("div", attributes);
    char* tag_str = (char*) WEBC_TagToString(tag);
    WEBC_AppendLn(buffer, tag_str);
    WEBC_CleanTag(&tag);
    free(tag_str);
}

WEBCAPI void WEBC_DivEnd(char** buffer)
{
    char* tag = WEBC_CLOSING_TAG("div");
    WEBC_AppendLn(buffer, tag);
    free(tag);
}
WEBCAPI void WEBC_MainStart(char** buffer, AttributeList attributes)
{
    Tag* tag = WEBC_MakeTag("main", attributes);
    char* tag_str = (char*) WEBC_TagToString(tag);
    WEBC_AppendLn(buffer, tag_str);
    WEBC_CleanTag(&tag);
    free(tag_str);
}

WEBCAPI void WEBC_MainEnd(char** buffer)
{
    char* tag = WEBC_CLOSING_TAG("main");
    WEBC_AppendLn(buffer, tag);
    free(tag);
}
WEBCAPI void WEBC_FooterStart(char** buffer, AttributeList attributes)
{
    Tag* tag = WEBC_MakeTag("footer", attributes);
    char* tag_str = (char*) WEBC_TagToString(tag);
    WEBC_AppendLn(buffer, tag_str);
    WEBC_CleanTag(&tag);
    free(tag_str);
}

WEBCAPI void WEBC_FooterEnd(char** buffer)
{
    char* tag = WEBC_CLOSING_TAG("footer");
    WEBC_AppendLn(buffer, tag);
    free(tag);
}

WEBCAPI void WEBC_ButtonStart(char** buffer, AttributeList attributes)
{
    Tag* tag = WEBC_MakeTag("button", attributes);
    char* tag_str = (char*) WEBC_TagToString(tag);
    WEBC_AppendLn(buffer, tag_str);
    WEBC_CleanTag(&tag);
    free(tag_str);
}

WEBCAPI void WEBC_ButtonEnd(char** buffer)
{
    char* tag = WEBC_CLOSING_TAG("button");
    WEBC_AppendLn(buffer, tag);
    free(tag);
}

WEBCAPI void WEBC_UlStart(char** buffer, AttributeList attributes)
{
    Tag* tag = WEBC_MakeTag("ul", attributes);
    char* tag_str = (char*) WEBC_TagToString(tag);
    WEBC_AppendLn(buffer, tag_str);
    WEBC_CleanTag(&tag);
    free(tag_str);
}

WEBCAPI void WEBC_UlEnd(char** buffer)
{
    char* tag = WEBC_CLOSING_TAG("ul");
    WEBC_AppendLn(buffer, tag);
    free(tag);
}

WEBCAPI void WEBC_Header(char **buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "header", attributes, contents);
}

WEBCAPI void WEBC_Heading(char** buffer, AttributeList attributes, size_t size, Cstr text){
    if(size == 0 || size > 6) {
        PANIC("Heading size should be between 1 and 6");
    }

    char* heading = clib_format_text("h%zu", size);
    WEBC_InlineBlock(buffer, heading, attributes, text);
    free(heading);
}

WEBCAPI void WEBC_Input(char** buffer, AttributeList attributes)
{
    Tag* tag = WEBC_MakeTag("input", attributes);
    char* tag_str = (char*) WEBC_TagToString(tag);
    WEBC_AppendLn(buffer, tag_str);
    free(tag_str);
    WEBC_CleanTag(&tag);
}

WEBCAPI void WEBC_Img(char** buffer, AttributeList attributes)
{
    Tag* tag = WEBC_MakeTag("img", attributes);
    char* tag_str = (char*) WEBC_TagToString(tag);
    WEBC_AppendLn(buffer, tag_str);
    free(tag_str);
    WEBC_CleanTag(&tag);
}

WEBCAPI void WEBC_Link(char** buffer, AttributeList attributes)
{
    Tag* tag = WEBC_MakeTag("link", attributes);
    char* tag_str = (char*) WEBC_TagToString(tag);
    WEBC_AppendLn(buffer, tag_str);
    free(tag_str);
    WEBC_CleanTag(&tag);
}

WEBCAPI void WEBC_Br(char** buffer)
{
    char* tag_str = (char*) WEBC_OPENING_TAG("br");
    WEBC_AppendLn(buffer, tag_str);
    free(tag_str);
}


WEBCAPI void WEBC_Hr(char** buffer)
{
    char* tag_str = (char*) WEBC_OPENING_TAG("hr");
    WEBC_AppendLn(buffer, tag_str);
    free(tag_str);
}

WEBCAPI void WEBC_Source(char** buffer, AttributeList attributes)
{
    Tag* tag = WEBC_MakeTag("source", attributes);
    char* tag_str = (char*) WEBC_TagToString(tag);
    WEBC_AppendLn(buffer, tag_str);
    free(tag_str);
    WEBC_CleanTag(&tag);
}

WEBCAPI void WEBC_Track(char** buffer, AttributeList attributes)
{
    Tag* tag = WEBC_MakeTag("track", attributes);
    char* tag_str = (char*) WEBC_TagToString(tag);
    WEBC_AppendLn(buffer, tag_str);
    free(tag_str);
    WEBC_CleanTag(&tag);
}

WEBCAPI void WEBC_Paragraph(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "p", attributes, text);
}

WEBCAPI void WEBC_ParagraphBlock(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "p", attributes, contents);
}

WEBCAPI void WEBC_Anchor(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "a", attributes, text);
}

WEBCAPI void WEBC_AnchorBlock(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "a", attributes, contents);
}

WEBCAPI void WEBC_Abbr(char** buffer, AttributeList attributes, Cstr abbr)
{
    WEBC_InlineBlock(buffer, "abbr", attributes, abbr);
}

WEBCAPI void WEBC_Address(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "address", attributes, contents);
}

WEBCAPI void WEBC_Div(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "div", attributes, contents);
}

WEBCAPI void WEBC_Style(char**buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "style", attributes, contents);
}

WEBCAPI void WEBC_Script(char**buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "script", attributes, contents);
}

WEBCAPI void WEBC_Cite(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "cite", attributes, text);
}

WEBCAPI void WEBC_Code(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "code", attributes, text);
}

WEBCAPI void WEBC_Bold(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "b", attributes, text);
}

WEBCAPI void WEBC_Blockquote(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "blockquote", attributes, text);
}

WEBCAPI void WEBC_Del(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "del", attributes, text);
}

WEBCAPI void WEBC_Li(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "li", attributes, text);
}

WEBCAPI void WEBC_Button(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "button", attributes, text);
}
WEBCAPI void WEBC_ButtonBlock(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "button", attributes, contents);
}

WEBCAPI void WEBC_Ul(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "ul", attributes, contents);
}

WEBCAPI void WEBC_Ol(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "ol", attributes, contents);
}

WEBCAPI void WEBC_Area(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "area", attributes, contents);
}

WEBCAPI void WEBC_Article(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "article", attributes, contents);
}

WEBCAPI void WEBC_Aside(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "aside", attributes, contents);
}

WEBCAPI void WEBC_Audio(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "audio", attributes, contents);
}

WEBCAPI void WEBC_Canvas(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "canvas", attributes, contents);
}

WEBCAPI void WEBC_Col(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "col", attributes, contents);
}

WEBCAPI void WEBC_Colgroup(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "colgroup", attributes, contents);
}

WEBCAPI void WEBC_Data(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "data", attributes, contents);
}

WEBCAPI void WEBC_Datalist(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "datalist", attributes, contents);
}

WEBCAPI void WEBC_Details(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "details", attributes, contents);
}

WEBCAPI void WEBC_Embed(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "embed", attributes, contents);
}

WEBCAPI void WEBC_Fieldset(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "fieldset", attributes, contents);
}

WEBCAPI void WEBC_Dialog(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "dialog", attributes, contents);
}

WEBCAPI void WEBC_Dl(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "dl", attributes, contents);
}

WEBCAPI void WEBC_Figure(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "figure", attributes, contents);
}

WEBCAPI void WEBC_Footer(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "footer", attributes, contents);
}

WEBCAPI void WEBC_Form (char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "form", attributes, contents);
}

WEBCAPI void WEBC_Iframe(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "iframe", attributes, contents);
}

WEBCAPI void WEBC_Hgroup(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "hgroup", attributes, contents);
}

WEBCAPI void WEBC_Main(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "main", attributes, contents);
}

WEBCAPI void WEBC_Map(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "map", attributes, contents);
}

WEBCAPI void WEBC_Menu(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "menu", attributes, contents);
}

WEBCAPI void WEBC_Nav(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "nav", attributes, contents);
}

WEBCAPI void WEBC_Noscript(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "noscript", attributes, contents);
}

WEBCAPI void WEBC_Object(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "object", attributes, contents);
}

WEBCAPI void WEBC_Optgroup(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "optgroup", attributes, contents);
}

WEBCAPI void WEBC_Picture(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "picture", attributes, contents);
}

WEBCAPI void WEBC_Ruby(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "ruby", attributes, contents);
}

WEBCAPI void WEBC_Search(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "search", attributes, contents);
}

WEBCAPI void WEBC_Section(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "section", attributes, contents);
}

WEBCAPI void WEBC_Select(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "select", attributes, contents);
}

WEBCAPI void WEBC_Svg(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "svg", attributes, contents);
}

WEBCAPI void WEBC_Table(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "table", attributes, contents);
}

WEBCAPI void WEBC_Tbody(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "tbody", attributes, contents);
}

WEBCAPI void WEBC_Tfoot(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "tfoot", attributes, contents);
}

WEBCAPI void WEBC_Thead(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "thead", attributes, contents);
}

WEBCAPI void WEBC_Tr(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "tr", attributes, contents);
}

WEBCAPI void WEBC_Video(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "video", attributes, contents);
}

WEBCAPI void WEBC_Template(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "template", attributes, contents);
}

WEBCAPI void WEBC_Textarea(char** buffer, AttributeList attributes, BlockContents contents)
{
    WEBC_BlockAttr(buffer, "textarea", attributes, contents);
}

WEBCAPI void WEBC_B(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "b", attributes, text);
}

WEBCAPI void WEBC_Bdo(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "bdo", attributes, text);
}

WEBCAPI void WEBC_Bdi(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "bdi", attributes, text);
}

WEBCAPI void WEBC_Caption(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "caption", attributes, text);
}

WEBCAPI void WEBC_Dd(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "dd", attributes, text);
}

WEBCAPI void WEBC_Dfn(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "dfn", attributes, text);
}

WEBCAPI void WEBC_Figcaption(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "figcaption", attributes, text);
}

WEBCAPI void WEBC_I(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "i", attributes, text);
}

WEBCAPI void WEBC_Dt(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "dt", attributes, text);
}

WEBCAPI void WEBC_Em(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "em", attributes, text);
}

WEBCAPI void WEBC_Ins(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "ins", attributes, text);
}

WEBCAPI void WEBC_Kbd(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "kbd", attributes, text);
}

WEBCAPI void WEBC_Label(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "label", attributes, text);
}

WEBCAPI void WEBC_Legend(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "legend", attributes, text);
}

WEBCAPI void WEBC_Meter(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "meter", attributes, text);
}

WEBCAPI void WEBC_Mark(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "mark", attributes, text);
}

WEBCAPI void WEBC_Option(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "option", attributes, text);
}

WEBCAPI void WEBC_Output(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "output", attributes, text);
}

WEBCAPI void WEBC_Param(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "param", attributes, text);
}

WEBCAPI void WEBC_Pre(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "pre", attributes, text);
}

WEBCAPI void WEBC_Progress(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "progress", attributes, text);
}

WEBCAPI void WEBC_Q(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "q", attributes, text);
}

WEBCAPI void WEBC_Rp(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "rp", attributes, text);
}

WEBCAPI void WEBC_Rt(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "rt", attributes, text);
}

WEBCAPI void WEBC_S(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "s", attributes, text);
}

WEBCAPI void WEBC_Samp(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "samp", attributes, text);
}

WEBCAPI void WEBC_Small(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "small", attributes, text);
}

WEBCAPI void WEBC_Span(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "span", attributes, text);
}

WEBCAPI void WEBC_Strong(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "strong", attributes, text);
}

WEBCAPI void WEBC_Sub(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "sub", attributes, text);
}

WEBCAPI void WEBC_Summary(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "summary", attributes, text);
}

WEBCAPI void WEBC_Time(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "time", attributes, text);
}

WEBCAPI void WEBC_Th(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "th", attributes, text);
}

WEBCAPI void WEBC_U(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "u", attributes, text);
}

WEBCAPI void WEBC_Var(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "var", attributes, text);
}

WEBCAPI void WEBC_Wbr(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "wbr", attributes, text);
}

WEBCAPI void WEBC_Sup(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "sup", attributes, text);
}

WEBCAPI void WEBC_Td(char** buffer, AttributeList attributes, Cstr text)
{
    WEBC_InlineBlock(buffer, "td", attributes, text);
}
