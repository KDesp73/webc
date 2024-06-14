#include "attribute.h"
#include "config.h"
#include "tags.h"
#include "elements.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define CLIB_IMPLEMENTATION
#include "clib.h"

void address_content(char** buffer){
    PlainText(buffer, "Written by ");
    Anchor(buffer, MakeAttributeList(MakeAttribute(HREF, "https://github.com/KDesp73"), MakeAttribute(TARGET, "_blank"), NULL), "KDesp73");
    Br(buffer);
    PlainText(buffer, "Source code at: ");
    Anchor(buffer, MakeAttributeList(MakeAttribute(HREF, "https://github.com/KDesp73/webc"), MakeAttribute(TARGET, "_blank"), NULL), "webc");
}

void text_demo(char** buffer){
    for (size_t i = 1; i <= 6; ++i) {
        Heading(buffer, MakeAttributeList(MakeAttribute(STYLE, "color: blue;"), NULL), i, clib_format_text("Heading %zu", i));
    }
    Paragraph(buffer, NULL, "Hello from C");
}

int main(void)
{
    Cstr output = "docs/index.html";
    char* buffer = NULL;

    HtmlStart(&buffer, "en");
    Head(&buffer, "WebC",
        MakeTag("meta",
            MakeAttribute(NAME, "author"),
            MakeAttribute(CONTENT, "Konstantinos Despoinidis"),
            NULL
        ),
        MakeTag("link",
            MakeAttribute(REL, "stylesheet"),
            MakeAttribute(HREF, "./style.css"),
            NULL
        ),
        NULL
    );

    BodyStart(&buffer);
        Div(
            &buffer,
            MakeAttributeList(MakeAttribute(STYLE, "background-color: green;"), NULL),
            text_demo
        );

        Input(&buffer, MakeAttribute(STYLE, "color: red;"), NULL);
        Br(&buffer);
        Hr(&buffer);
        Abbr(&buffer, MakeAttributeList(MakeAttribute(TITLE, "World Health Organization"), NULL), "WHO");

        Address(&buffer, NULL, address_content);

    BodyEnd(&buffer);
    HtmlEnd(&buffer);

    Export(buffer, output);
    Clean(&buffer);
    return 0;
}

