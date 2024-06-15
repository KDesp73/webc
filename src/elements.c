#include "webc.h"
#include <assert.h>
#include <stdlib.h>

WEBCAPI void BlockAttr(char** buffer, Cstr name, Attribute** attributes, BlockContents contents)
{
    Tag* tag = (Tag*) malloc(sizeof(Tag));
    tag->name = name;
    tag->attributes = attributes;

    tag->attr_count = 0;
    if(attributes != NULL)
        while(attributes[tag->attr_count] != NULL) tag->attr_count++;
    tag->attr_capacity = tag->attr_count;

    Block(buffer, tag, contents);
}

WEBCAPI void Block(char** buffer, Tag* tag, BlockContents contents)
{
    assert(tag != NULL);

    Append(buffer, TagToString(tag));
    contents(buffer);
    Append(buffer, ClosingTag(tag));
    CleanTag(tag);
}

WEBCAPI void InlineBlock(char** buffer, Cstr name, Attribute** attributes, Cstr text)
{

    Tag* tag = (Tag*) malloc(sizeof(Tag));

    if(tag == NULL) {
        PANIC("Couldn't allocate tag");
    }    

    tag->name = name;
    tag->attributes = attributes;

    tag->attr_count = 0;
    if(attributes != NULL)
        while(attributes[tag->attr_count] != NULL) tag->attr_count++;

    if(tag->attr_count > 5) tag->attr_count = 0;
    // DEBU("tag: %s, count: %zu", tag->name, tag->attr_count);

    tag->attr_capacity = tag->attr_count;

    Append(buffer, clib_format_text("%s%s%s", TagToString(tag), text, ClosingTag(tag)));
    CleanTag(tag);
}

/* ######################### Elements Below ######################### */

WEBCAPI void Head(char** buffer, Cstr title, Tag* first, ...)
{
    CstrArray header_arr = clib_cstr_array_make(
        "<head>",
        "<meta charset=\"UTF-8\">",
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">",
        NULL
    );

    char* header = (char*) clib_cstr_array_join("\n", header_arr);

    if (first != NULL) {
        char* first_tag_str = (char*) TagToString(first);
        strcat(header, "\n");
        strcat(header, first_tag_str);
        free(first_tag_str);

        va_list args;
        va_start(args, first);
        Tag* tag = va_arg(args, Tag*);
        while (tag != NULL) {
            char* tag_str = (char*) TagToString(tag);
            strcat(header, "\n");
            strcat(header, tag_str);
            free(tag_str);
            tag = va_arg(args, Tag*);
        }
        va_end(args);
    }

    Cstr title_tag = clib_format_text("<title>%s</title>", title);
    Cstr header_str = clib_format_text("%s\n%s\n</head>", header, title_tag);
    Append(buffer, header_str);

    free(header);
}

WEBCAPI void HtmlStart(char**buffer, Cstr lang)
{
    if(lang == NULL){
        PANIC("Language is NULL");
    }

    *buffer = (char*) malloc(MAX_BUFFER_SIZE);
    if(*buffer == NULL){
        PANIC("Couldnt initialize buffer");
    }
    *buffer[0] = '\0';

    char* temp = (char*) JOIN("\n",
        "<!DOCTYPE html>",
        CONCAT("<html lang=\"", lang, "\">\n")
    );

    *buffer = (char*) realloc(*buffer, strlen(temp) + 1);
    if(*buffer == NULL){
        PANIC("Couldnt reallocate buffer");
    }

    strcpy(*buffer, temp);
    free(temp);
}

WEBCAPI void HtmlEnd(char** buffer)
{
    Append(buffer, CLOSING_TAG("html"));
}


WEBCAPI void BodyStart(char** buffer)
{
    Append(buffer, OPENING_TAG("body"));
}

WEBCAPI void BodyEnd(char** buffer)
{
    Append(buffer, CLOSING_TAG("body"));
}

WEBCAPI void ScriptStart(char** buffer)
{
    Append(buffer, OPENING_TAG("script"));
}

WEBCAPI void ScriptEnd(char** buffer)
{
    Append(buffer, CLOSING_TAG("script"));
}

WEBCAPI void StyleStart(char** buffer)
{
    Append(buffer, OPENING_TAG("style"));
}

WEBCAPI void StyleEnd(char** buffer)
{
    Append(buffer, CLOSING_TAG("style"));
}

WEBCAPI void Heading(char** buffer, Attribute** attributes, size_t size, Cstr text){
    if(size == 0 || size > 6) {
        PANIC("Heading size should be between 1 and 6");
    }

    InlineBlock(buffer, clib_format_text("h%zu", size), attributes, text);
}

WEBCAPI void Paragraph(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "p", attributes, text);
}

WEBCAPI void ParagraphBlock(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "p", attributes, contents);
}

WEBCAPI void Anchor(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "a", attributes, text);
}

WEBCAPI void AnchorBlock(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "a", attributes, contents);
}

WEBCAPI void Abbr(char** buffer, Attribute** attributes, Cstr abbr)
{
    InlineBlock(buffer, "abbr", attributes, abbr);
}

WEBCAPI void Address(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "address", attributes, contents);
}

WEBCAPI void Div(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "div", attributes, contents);
}

WEBCAPI void Cite(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "cite", attributes, text);
}

WEBCAPI void Code(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "code", attributes, text);
}

WEBCAPI void Bold(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "b", attributes, text);
}

WEBCAPI void Blockquote(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "blockquote", attributes, text);
}

WEBCAPI void Del(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "del", attributes, text);
}

WEBCAPI void Li(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "li", attributes, text);
}

WEBCAPI void Ul(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "ul", attributes, contents);
}


WEBCAPI void Ol(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "ol", attributes, contents);
}

WEBCAPI void Area(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "area", attributes, contents);
}
WEBCAPI void Article(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "article", attributes, contents);
}
WEBCAPI void Aside(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "aside", attributes, contents);
}
WEBCAPI void Audio(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "audio", attributes, contents);
}
WEBCAPI void Canvas(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "canvas", attributes, contents);
}
WEBCAPI void Col(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "col", attributes, contents);
}
WEBCAPI void Colgroup(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "colgroup", attributes, contents);
}
WEBCAPI void Data(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "data", attributes, contents);
}
WEBCAPI void Datalist(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "datalist", attributes, contents);
}
WEBCAPI void Details(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "details", attributes, contents);
}
WEBCAPI void Embed(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "embed", attributes, contents);
}
WEBCAPI void Fieldset(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "fieldset", attributes, contents);
}
WEBCAPI void Dialog(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "dialog", attributes, contents);
}
WEBCAPI void Dl(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "dl", attributes, contents);
}
WEBCAPI void Figure(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "figure", attributes, contents);
}
WEBCAPI void Footer(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "footer", attributes, contents);
}
WEBCAPI void Form (char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "form", attributes, contents);
}
WEBCAPI void Iframe(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "iframe", attributes, contents);
}
WEBCAPI void Hgroup(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "hgroup", attributes, contents);
}
WEBCAPI void Main(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "main", attributes, contents);
}
WEBCAPI void Map(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "map", attributes, contents);
}
WEBCAPI void Menu(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "menu", attributes, contents);
}
WEBCAPI void Nav(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "nav", attributes, contents);
}
WEBCAPI void Noscript(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "noscript", attributes, contents);
}
WEBCAPI void Object(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "object", attributes, contents);
}
WEBCAPI void Optgroup(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "optgroup", attributes, contents);
}
WEBCAPI void Picture(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "picture", attributes, contents);
}
WEBCAPI void Ruby(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "ruby", attributes, contents);
}
WEBCAPI void Search(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "search", attributes, contents);
}
WEBCAPI void Section(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "section", attributes, contents);
}
WEBCAPI void Select(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "select", attributes, contents);
}
WEBCAPI void Svg(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "svg", attributes, contents);
}
WEBCAPI void Table(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "table", attributes, contents);
}
WEBCAPI void Tbody(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "tbody", attributes, contents);
}
WEBCAPI void Tfoot(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "tfoot", attributes, contents);
}
WEBCAPI void Thead(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "thead", attributes, contents);
}
WEBCAPI void Tr(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "tr", attributes, contents);
}
WEBCAPI void Video(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "video", attributes, contents);
}
WEBCAPI void Template(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "template", attributes, contents);
}
WEBCAPI void Textarea(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "textarea", attributes, contents);
}
WEBCAPI void B(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "b", attributes, text);
}
WEBCAPI void Bdo(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "bdo", attributes, text);
}
WEBCAPI void Bdi(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "bdi", attributes, text);
}
WEBCAPI void Caption(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "caption", attributes, text);
}
WEBCAPI void Dd(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "dd", attributes, text);
}
WEBCAPI void Dfn(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "dfn", attributes, text);
}
WEBCAPI void Figcaption(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "figcaption", attributes, text);
}
WEBCAPI void I(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "i", attributes, text);
}
WEBCAPI void Dt(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "dt", attributes, text);
}
WEBCAPI void Em(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "em", attributes, text);
}
WEBCAPI void Ins(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "ins", attributes, text);
}
WEBCAPI void Kbd(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "kbd", attributes, text);
}
WEBCAPI void Label(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "label", attributes, text);
}
WEBCAPI void Legend(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "legend", attributes, text);
}
WEBCAPI void Meter(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "meter", attributes, text);
}
WEBCAPI void Mark(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "mark", attributes, text);
}
WEBCAPI void Option(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "option", attributes, text);
}
WEBCAPI void Output(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "output", attributes, text);
}
WEBCAPI void Param(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "param", attributes, text);
}
WEBCAPI void Pre(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "pre", attributes, text);
}
WEBCAPI void Progress(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "progress", attributes, text);
}
WEBCAPI void Q(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "q", attributes, text);
}
WEBCAPI void Rp(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "rp", attributes, text);
}
WEBCAPI void Rt(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "rt", attributes, text);
}
WEBCAPI void S(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "s", attributes, text);
}
WEBCAPI void Samp(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "samp", attributes, text);
}
WEBCAPI void Small(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "small", attributes, text);
}
WEBCAPI void Span(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "span", attributes, text);
}
WEBCAPI void Strong(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "strong", attributes, text);
}
WEBCAPI void Sub(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "sub", attributes, text);
}
WEBCAPI void Summary(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "summary", attributes, text);
}
WEBCAPI void Time(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "time", attributes, text);
}
WEBCAPI void Th(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "th", attributes, text);
}
WEBCAPI void U(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "u", attributes, text);
}
WEBCAPI void Var(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "var", attributes, text);
}
WEBCAPI void Wbr(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "wbr", attributes, text);
}
WEBCAPI void Sup(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "sup", attributes, text);
}
WEBCAPI void Td(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "td", attributes, text);
}
