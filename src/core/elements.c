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
    if(contents != NULL)
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
    WEBC_TagEnd(buffer, "html");
}

WEBCAPI void WEBC_TagStart(char** buffer, Cstr tag_name, AttributeList attributes)
{
    Tag* tag = WEBC_MakeTag(tag_name, attributes);
    char* tag_str = (char*) WEBC_TagToString(tag);
    WEBC_AppendLn(buffer, tag_str);
    WEBC_CleanTag(&tag);
    free(tag_str);
}

WEBCAPI void WEBC_TagEnd(char** buffer, Cstr tag_name)
{
    char* tag = WEBC_CLOSING_TAG(tag_name);
    WEBC_AppendLn(buffer, tag);
    free(tag);
}

WEBCAPI void WEBC_BodyStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "body", attributes);
}
WEBCAPI void WEBC_BodyEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "body");
}
WEBCAPI void WEBC_AbbrStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "abbr", attributes);
}
WEBCAPI void WEBC_AbbrEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "abbr");
}
WEBCAPI void WEBC_AddressStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "address", attributes);
}
WEBCAPI void WEBC_AddressEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "address");
}
WEBCAPI void WEBC_AnchorStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "anchor", attributes);
}
WEBCAPI void WEBC_AnchorEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "anchor");
}
WEBCAPI void WEBC_AreaStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "area", attributes);
}
WEBCAPI void WEBC_AreaEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "area");
}
WEBCAPI void WEBC_ArticleStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "article", attributes);
}
WEBCAPI void WEBC_ArticleEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "article");
}
WEBCAPI void WEBC_AsideStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "aside", attributes);
}
WEBCAPI void WEBC_AsideEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "aside");
}
WEBCAPI void WEBC_AudioStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "audio", attributes);
}
WEBCAPI void WEBC_AudioEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "audio");
}
WEBCAPI void WEBC_BStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "B", attributes);
}
WEBCAPI void WEBC_BEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "B");
}
WEBCAPI void WEBC_BdiStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "bdi", attributes);
}
WEBCAPI void WEBC_BdiEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "bdi");
}
WEBCAPI void WEBC_BdoStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "bdo", attributes);
}
WEBCAPI void WEBC_BdoEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "bdo");
}
WEBCAPI void WEBC_BlockquoteStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "blockquote", attributes);
}
WEBCAPI void WEBC_BlockquoteEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "blockquote");
}
WEBCAPI void WEBC_BoldStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "bold", attributes);
}
WEBCAPI void WEBC_BoldEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "bold");
}
WEBCAPI void WEBC_BrStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "br", attributes);
}
WEBCAPI void WEBC_BrEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "br");
}
WEBCAPI void WEBC_ButtonStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "button", attributes);
}
WEBCAPI void WEBC_ButtonEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "button");
}
WEBCAPI void WEBC_CanvasStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "canvas", attributes);
}
WEBCAPI void WEBC_CanvasEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "canvas");
}
WEBCAPI void WEBC_CaptionStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "caption", attributes);
}
WEBCAPI void WEBC_CaptionEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "caption");
}
WEBCAPI void WEBC_CiteStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "cite", attributes);
}
WEBCAPI void WEBC_CiteEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "cite");
}
WEBCAPI void WEBC_CodeStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "code", attributes);
}
WEBCAPI void WEBC_CodeEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "code");
}
WEBCAPI void WEBC_ColStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "col", attributes);
}
WEBCAPI void WEBC_ColEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "col");
}
WEBCAPI void WEBC_ColgroupStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "colgroup", attributes);
}
WEBCAPI void WEBC_ColgroupEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "colgroup");
}
WEBCAPI void WEBC_DataStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "data", attributes);
}
WEBCAPI void WEBC_DataEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "data");
}
WEBCAPI void WEBC_DatalistStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "datalist", attributes);
}
WEBCAPI void WEBC_DatalistEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "datalist");
}
WEBCAPI void WEBC_DdStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "dd", attributes);
}
WEBCAPI void WEBC_DdEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "dd");
}
WEBCAPI void WEBC_DelStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "del", attributes);
}
WEBCAPI void WEBC_DelEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "del");
}
WEBCAPI void WEBC_DetailsStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "details", attributes);
}
WEBCAPI void WEBC_DetailsEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "details");
}
WEBCAPI void WEBC_DfnStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "dfn", attributes);
}
WEBCAPI void WEBC_DfnEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "dfn");
}
WEBCAPI void WEBC_DialogStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "dialog", attributes);
}
WEBCAPI void WEBC_DialogEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "dialog");
}
WEBCAPI void WEBC_DivStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "div", attributes);
}
WEBCAPI void WEBC_DivEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "div");
}
WEBCAPI void WEBC_DlStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "dl", attributes);
}
WEBCAPI void WEBC_DlEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "dl");
}
WEBCAPI void WEBC_DtStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "dt", attributes);
}
WEBCAPI void WEBC_DtEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "dt");
}
WEBCAPI void WEBC_EmStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "em", attributes);
}
WEBCAPI void WEBC_EmEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "em");
}
WEBCAPI void WEBC_EmbedStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "embed", attributes);
}
WEBCAPI void WEBC_EmbedEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "embed");
}
WEBCAPI void WEBC_FieldsetStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "fieldset", attributes);
}
WEBCAPI void WEBC_FieldsetEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "fieldset");
}
WEBCAPI void WEBC_FigcaptionStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "figcaption", attributes);
}
WEBCAPI void WEBC_FigcaptionEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "figcaption");
}
WEBCAPI void WEBC_FigureStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "figure", attributes);
}
WEBCAPI void WEBC_FigureEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "figure");
}
WEBCAPI void WEBC_FooterStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "footer", attributes);
}
WEBCAPI void WEBC_FooterEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "footer");
}
WEBCAPI void WEBC_FormStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "form", attributes);
}
WEBCAPI void WEBC_FormEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "form");
}
WEBCAPI void WEBC_HeaderStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "header", attributes);
}
WEBCAPI void WEBC_HeaderEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "header");
}
WEBCAPI void WEBC_HeadingStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "heading", attributes);
}
WEBCAPI void WEBC_HeadingEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "heading");
}
WEBCAPI void WEBC_HgroupStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "hgroup", attributes);
}
WEBCAPI void WEBC_HgroupEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "hgroup");
}
WEBCAPI void WEBC_HrStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "hr", attributes);
}
WEBCAPI void WEBC_HrEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "hr");
}
WEBCAPI void WEBC_IStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "i", attributes);
}
WEBCAPI void WEBC_IEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "i");
}
WEBCAPI void WEBC_IframeStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "iframe", attributes);
}
WEBCAPI void WEBC_IframeEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "iframe");
}
WEBCAPI void WEBC_ImgStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "img", attributes);
}
WEBCAPI void WEBC_ImgEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "img");
}
WEBCAPI void WEBC_InputStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "input", attributes);
}
WEBCAPI void WEBC_InputEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "input");
}
WEBCAPI void WEBC_InsStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "ins", attributes);
}
WEBCAPI void WEBC_InsEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "ins");
}
WEBCAPI void WEBC_KbdStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "kbd", attributes);
}
WEBCAPI void WEBC_KbdEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "kbd");
}
WEBCAPI void WEBC_LabelStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "label", attributes);
}
WEBCAPI void WEBC_LabelEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "label");
}
WEBCAPI void WEBC_LegendStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "legend", attributes);
}
WEBCAPI void WEBC_LegendEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "legend");
}
WEBCAPI void WEBC_LiStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "li", attributes);
}
WEBCAPI void WEBC_LiEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "li");
}
WEBCAPI void WEBC_LinkStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "link", attributes);
}
WEBCAPI void WEBC_LinkEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "link");
}
WEBCAPI void WEBC_MainStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "main", attributes);
}
WEBCAPI void WEBC_MainEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "main");
}
WEBCAPI void WEBC_MapStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "map", attributes);
}
WEBCAPI void WEBC_MapEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "map");
}
WEBCAPI void WEBC_MarkStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "mark", attributes);
}
WEBCAPI void WEBC_MarkEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "mark");
}
WEBCAPI void WEBC_MenuStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "menu", attributes);
}
WEBCAPI void WEBC_MenuEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "menu");
}
WEBCAPI void WEBC_MeterStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "meter", attributes);
}
WEBCAPI void WEBC_MeterEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "meter");
}
WEBCAPI void WEBC_NavStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "nav", attributes);
}
WEBCAPI void WEBC_NavEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "nav");
}
WEBCAPI void WEBC_NoscriptStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "noscript", attributes);
}
WEBCAPI void WEBC_NoscriptEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "noscript");
}
WEBCAPI void WEBC_ObjectStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "object", attributes);
}
WEBCAPI void WEBC_ObjectEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "object");
}
WEBCAPI void WEBC_OlStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "ol", attributes);
}
WEBCAPI void WEBC_OlEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "ol");
}
WEBCAPI void WEBC_OptgroupStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "optgroup", attributes);
}
WEBCAPI void WEBC_OptgroupEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "optgroup");
}
WEBCAPI void WEBC_OptionStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "option", attributes);
}
WEBCAPI void WEBC_OptionEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "option");
}
WEBCAPI void WEBC_OutputStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "output", attributes);
}
WEBCAPI void WEBC_OutputEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "output");
}
WEBCAPI void WEBC_ParagraphStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "paragraph", attributes);
}
WEBCAPI void WEBC_ParagraphEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "paragraph");
}
WEBCAPI void WEBC_ParamStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "param", attributes);
}
WEBCAPI void WEBC_ParamEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "param");
}
WEBCAPI void WEBC_PictureStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "picture", attributes);
}
WEBCAPI void WEBC_PictureEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "picture");
}
WEBCAPI void WEBC_PreStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "pre", attributes);
}
WEBCAPI void WEBC_PreEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "pre");
}
WEBCAPI void WEBC_ProgressStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "progress", attributes);
}
WEBCAPI void WEBC_ProgressEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "progress");
}
WEBCAPI void WEBC_QStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "Q", attributes);
}
WEBCAPI void WEBC_QEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "Q");
}
WEBCAPI void WEBC_RpStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "rp", attributes);
}
WEBCAPI void WEBC_RpEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "rp");
}
WEBCAPI void WEBC_RtStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "rt", attributes);
}
WEBCAPI void WEBC_RtEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "rt");
}
WEBCAPI void WEBC_RubyStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "ruby", attributes);
}
WEBCAPI void WEBC_RubyEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "ruby");
}
WEBCAPI void WEBC_SStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "s", attributes);
}
WEBCAPI void WEBC_SEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "s");
}
WEBCAPI void WEBC_SampStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "samp", attributes);
}
WEBCAPI void WEBC_SampEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "samp");
}
WEBCAPI void WEBC_ScriptStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "script", attributes);
}
WEBCAPI void WEBC_ScriptEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "script");
}
WEBCAPI void WEBC_SearchStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "search", attributes);
}
WEBCAPI void WEBC_SearchEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "search");
}
WEBCAPI void WEBC_SectionStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "section", attributes);
}
WEBCAPI void WEBC_SectionEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "section");
}
WEBCAPI void WEBC_SelectStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "select", attributes);
}
WEBCAPI void WEBC_SelectEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "select");
}
WEBCAPI void WEBC_SmallStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "small", attributes);
}
WEBCAPI void WEBC_SmallEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "small");
}
WEBCAPI void WEBC_SourceStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "source", attributes);
}
WEBCAPI void WEBC_SourceEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "source");
}
WEBCAPI void WEBC_SpanStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "span", attributes);
}
WEBCAPI void WEBC_SpanEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "span");
}
WEBCAPI void WEBC_StrongStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "strong", attributes);
}
WEBCAPI void WEBC_StrongEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "strong");
}
WEBCAPI void WEBC_StyleStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "style", attributes);
}
WEBCAPI void WEBC_StyleEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "style");
}
WEBCAPI void WEBC_SubStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "sub", attributes);
}
WEBCAPI void WEBC_SubEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "sub");
}
WEBCAPI void WEBC_SummaryStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "summary", attributes);
}
WEBCAPI void WEBC_SummaryEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "summary");
}
WEBCAPI void WEBC_SupStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "sup", attributes);
}
WEBCAPI void WEBC_SupEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "sup");
}
WEBCAPI void WEBC_SvgStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "svg", attributes);
}
WEBCAPI void WEBC_SvgEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "svg");
}
WEBCAPI void WEBC_TableStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "table", attributes);
}
WEBCAPI void WEBC_TableEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "table");
}
WEBCAPI void WEBC_TbodyStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "tbody", attributes);
}
WEBCAPI void WEBC_TbodyEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "tbody");
}
WEBCAPI void WEBC_TdStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "td", attributes);
}
WEBCAPI void WEBC_TdEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "td");
}
WEBCAPI void WEBC_TemplateStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "template", attributes);
}
WEBCAPI void WEBC_TemplateEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "template");
}
WEBCAPI void WEBC_TextareaStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "textarea", attributes);
}
WEBCAPI void WEBC_TextareaEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "textarea");
}
WEBCAPI void WEBC_TfootStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "tfoot", attributes);
}
WEBCAPI void WEBC_TfootEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "tfoot");
}
WEBCAPI void WEBC_ThStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "th", attributes);
}
WEBCAPI void WEBC_ThEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "th");
}
WEBCAPI void WEBC_TheadStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "thead", attributes);
}
WEBCAPI void WEBC_TheadEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "thead");
}
WEBCAPI void WEBC_TimeStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "time", attributes);
}
WEBCAPI void WEBC_TimeEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "time");
}
WEBCAPI void WEBC_TrStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "tr", attributes);
}
WEBCAPI void WEBC_TrEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "tr");
}
WEBCAPI void WEBC_TrackStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "track", attributes);
}
WEBCAPI void WEBC_TrackEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "track");
}
WEBCAPI void WEBC_UStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "u", attributes);
}
WEBCAPI void WEBC_UEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "u");
}
WEBCAPI void WEBC_UlStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "ul", attributes);
}
WEBCAPI void WEBC_UlEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "ul");
}
WEBCAPI void WEBC_VarStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "var", attributes);
}
WEBCAPI void WEBC_VarEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "var");
}
WEBCAPI void WEBC_VideoStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "video", attributes);
}
WEBCAPI void WEBC_VideoEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "video");
}
WEBCAPI void WEBC_WbrStart(char** buffer, AttributeList attributes)
{
    WEBC_TagStart(buffer, "wbr", attributes);
}
WEBCAPI void WEBC_WbrEnd(char** buffer)
{
    WEBC_TagEnd(buffer, "wbr");
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
