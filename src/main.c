#include "attribute.h"
#include "config.h"
#include "tags.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define CLIB_IMPLEMENTATION
#include "clib.h"

void address_content(char** buffer){
    PlainText(buffer, "Written by KDesp73");
    Br(buffer);
    PlainText(buffer, "Visit Us at: ");
    Br(buffer);
    PlainText(buffer, "Example.com");
}

int main(void)
{
    Cstr output = "index.html";
    char* buffer = NULL;

    HtmlStart(&buffer, "en");
    Head(&buffer, "WebC",
        MakeTag("meta",
            MakeAttribute(NAME, "author"),
            MakeAttribute(CONTENT, "Konstantinos Despoinidis"),
            NULL
        ),
        MakeTag("link",
            MakeAttribute(HREF, "./style.css"),
            NULL
        ),
        NULL
    );

    BodyStart(&buffer);
        for (size_t i = 1; i <= 6; ++i) {
            Heading(&buffer, MakeAttributeList(MakeAttribute(STYLE, "color: blue;"), NULL), i, clib_format_text("Heading %zu", i));
        }
        Paragraph(&buffer, NULL, "Hello from C");

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

