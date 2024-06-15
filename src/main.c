#include "webc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void address_content(char** buffer)
{
    PlainText(buffer, "Written by ");
    Anchor(buffer, MakeAttributeList(MakeAttribute(ATTR_HREF, "https://github.com/KDesp73"), MakeAttribute(ATTR_TARGET, "_blank"), NULL), "KDesp73");
    Br(buffer);
    PlainText(buffer, "Source code at: ");
    Anchor(buffer, MakeAttributeList(MakeAttribute(ATTR_HREF, "https://github.com/KDesp73/webc"), MakeAttribute(ATTR_TARGET, "_blank"), NULL), "webc");
}

void text_demo(char** buffer){
    for (size_t i = 1; i <= 6; ++i) {
        Heading(buffer, NO_ATTRIBUTES, i, clib_format_text("Heading %zu", i));
    }
    Paragraph(buffer, NO_ATTRIBUTES, "Hello from C");
}

void list_fruits(char** buffer)
{
    CstrArray fruits = clib_cstr_array_make(
        "apple", "banana", "cherry", "watermelon", "pear", NULL
    );
    CstrArray colors = clib_cstr_array_make(
        "red", "yellow", "red", "green", "lightgreen", NULL
    );

    for(size_t i = 0; i < fruits.count; ++i){
        Li(buffer, 
            MakeAttributeList(
                MakeAttribute(ATTR_STYLE, clib_format_text("color: %s;", colors.items[i])), 
                MakeAttribute(ATTR_CLASS, "item"), NULL
            ), 
            fruits.items[i]
        );
    }
}

char* Index()
{
    char* buffer = NULL;

    HtmlStart(&buffer, "en");
    Head(&buffer, "WebC Example",
        MakeTag(META,
            MakeAttributeList(
                MakeAttribute(ATTR_NAME, "author"),
                MakeAttribute(ATTR_CONTENT, "Konstantinos Despoinidis"),
                NULL
            )
        ),
        MakeTag(LINK,
            MakeAttributeList(
                MakeAttribute(ATTR_REL, "stylesheet"),
                MakeAttribute(ATTR_HREF, "./style.css"),
                NULL
            )
        ),
        MakeTag(BASE, 
            MakeAttributeList(
                MakeAttribute(ATTR_HREF, "https://www.github.com"),
                MakeAttribute(ATTR_TARGET, "_blank"),
                NULL
            )
        ),
        NULL
    );

    ScriptStart(&buffer);
        Javascript(&buffer, "console.log('Hello World!');");
        Javascript(&buffer, "console.log('Hello World!');");
        Javascript(&buffer, "console.log('Hello World!');");
        Javascript(&buffer, "console.log('Hello World!');");
    ScriptEnd(&buffer);


    BodyStart(&buffer);
        Div(
            &buffer,
            MakeAttributeList(MakeAttribute(ATTR_STYLE, "background-color: grey;"), NULL),
            text_demo
        );

        Input(&buffer, MakeAttributeList(MakeAttribute(ATTR_STYLE, "color: red;"), NULL));
        Br(&buffer);
        Hr(&buffer);
        Abbr(&buffer, MakeAttributeList(MakeAttribute(ATTR_TITLE, "World Health Organization"), NULL), "WHO");

        Address(&buffer, NO_ATTRIBUTES, address_content);
        Img(&buffer, 
            MakeAttributeList(
                MakeAttribute(ATTR_SRC, "https://letsenhance.io/static/8f5e523ee6b2479e26ecc91b9c25261e/1015f/MainAfter.jpg"),
                MakeAttribute(ATTR_ALT, "chameleon"),
                MakeAttribute(ATTR_WIDTH, "500"),
                MakeAttribute(ATTR_HEIGHT, "300"),
                NULL
            )
        );

        Ul(&buffer, NO_ATTRIBUTES, list_fruits);
        text_demo(&buffer);

    BodyEnd(&buffer);
    HtmlEnd(&buffer);

    return buffer;
}

char* About()
{
    char* buffer = NULL;
    HtmlStart(&buffer, "en");
    Head(&buffer, "About",
        MakeTag(META, 
            MakeAttributeList(
                MakeAttribute(ATTR_NAME, "author"),
                MakeAttribute(ATTR_CONTENT, "Konstantinos Despoinidis"),
                NULL
            )
        ),
        MakeTag(LINK,
            MakeAttributeList(
                MakeAttribute(ATTR_REL, "stylesheet"),
                MakeAttribute(ATTR_HREF, "../style.css"),
                NULL
            )
        ),
        MakeTag(BASE, 
            MakeAttributeList(
                MakeAttribute(ATTR_HREF, "https://www.github.com"),
                MakeAttribute(ATTR_TARGET, "_blank"),
                NULL
            )
        ),
        NULL
    );

    BodyStart(&buffer);
    
        Heading(&buffer, NO_ATTRIBUTES, 1, "About");
        Anchor(&buffer, MakeAttributeList(MakeAttribute(ATTR_HREF, "KDesp73"), NULL), "Github");

    BodyEnd(&buffer);

    return buffer;
}

int main(int argc, char** argv)
{
    Cstr root = "site/";
    Route index = {
        .path = root,
        .buffer = Index()
    };
    
    Route about = {
        .path = CONCAT(root, "about/"),
        .buffer = About()
    };

    ExportRoute(index);
    ExportRoute(about);

    return 0;
}

