#include "webc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define CLIB_IMPLEMENTATION
#include "clib.h"

void address_content(char** buffer){
    PlainText(buffer, "Written by ");
    Anchor(buffer, MakeAttributeList(MakeAttribute(ATTR_HREF, "https://github.com/KDesp73"), MakeAttribute(ATTR_TARGET, "_blank"), NULL), "KDesp73");
    Br(buffer);
    PlainText(buffer, "Source code at: ");
    Anchor(buffer, MakeAttributeList(MakeAttribute(ATTR_HREF, "https://github.com/KDesp73/webc"), MakeAttribute(ATTR_TARGET, "_blank"), NULL), "webc");
}

void text_demo(char** buffer){
    for (size_t i = 1; i <= 6; ++i) {
        Heading(buffer, NULL, i, clib_format_text("Heading %zu", i));
    }
    Paragraph(buffer, NULL, "Hello from C");
}

void list_fruits(char** buffer){
    CstrArray fruits = clib_cstr_array_make(
        "apple", "banana", "cherry", "watermelon", "pear", NULL
    );
    CstrArray colors = clib_cstr_array_make(
        "red", "yellow", "red", "green", "lightgreen", NULL
    );

    for(size_t i = 0; i < fruits.count; ++i){
        Li(buffer, MakeAttributeList(MakeAttribute(ATTR_STYLE, clib_format_text("color: %s;", colors.items[i])), NULL), fruits.items[i]);
    }
}

char* Index(){
    char* buffer = NULL;

    HtmlStart(&buffer, "en");
    Head(&buffer, "WebC Example",
        MakeTag("meta",
            MakeAttribute(ATTR_NAME, "author"),
            MakeAttribute(ATTR_CONTENT, "Konstantinos Despoinidis"),
            NULL
        ),
        MakeTag("link",
            MakeAttribute(ATTR_REL, "stylesheet"),
            MakeAttribute(ATTR_HREF, "./style.css"),
            NULL
        ),
        NULL
    );

    ScriptStart(&buffer);
        Javascript(&buffer, "console.log('Hello World!')");
    ScriptEnd(&buffer);

    BodyStart(&buffer);
        Div(
            &buffer,
            MakeAttributeList(MakeAttribute(ATTR_STYLE, "background-color: grey;"), NULL),
            text_demo
        );

        Input(&buffer, MakeAttribute(ATTR_STYLE, "color: red;"), NULL);
        Br(&buffer);
        Hr(&buffer);
        Abbr(&buffer, MakeAttributeList(MakeAttribute(ATTR_TITLE, "World Health Organization"), NULL), "WHO");

        Address(&buffer, NULL, address_content);
        Img(&buffer, 
            MakeAttribute(ATTR_SRC, "https://letsenhance.io/static/8f5e523ee6b2479e26ecc91b9c25261e/1015f/MainAfter.jpg"),
            MakeAttribute(ATTR_ALT, "chameleon"),
            MakeAttribute(ATTR_WIDTH, "500"),
            MakeAttribute(ATTR_HEIGHT, "300"),
            NULL
        );

        Ul(&buffer, NULL, list_fruits);
        text_demo(&buffer);

    BodyEnd(&buffer);
    HtmlEnd(&buffer);

    return buffer;
}

char* About(){
    char* buffer = NULL;
    HtmlStart(&buffer, "en");
    Head(&buffer, "About",
        MakeTag("meta",
            MakeAttribute(ATTR_NAME, "author"),
            MakeAttribute(ATTR_CONTENT, "Konstantinos Despoinidis"),
            NULL
        ),
        MakeTag("link",
            MakeAttribute(ATTR_REL, "stylesheet"),
            MakeAttribute(ATTR_HREF, "./style.css"),
            NULL
        ),
        NULL
    );

    BodyStart(&buffer);
    
        Heading(&buffer, NULL, 1, "About");

    BodyEnd(&buffer);

    return buffer;
}

int main(void)
{
    Route index = {
        .path = "site/index.html",
        .buffer = Index()
    };
    
    Route about = {
        .path = "site/about.html",
        .buffer = About()
    };

    ExportRoute(index);
    ExportRoute(about);

    return 0;
}

