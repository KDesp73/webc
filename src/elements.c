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
    DEBU("tag: %s, count: %zu", tag->name, tag->attr_count);

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

WEBCAPI void ParagraphEx(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "p", attributes, contents);
}

WEBCAPI void Anchor(char** buffer, Attribute** attributes, Cstr text)
{
    InlineBlock(buffer, "a", attributes, text);
}

WEBCAPI void AnchorEx(char** buffer, Attribute** attributes, BlockContents contents)
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

