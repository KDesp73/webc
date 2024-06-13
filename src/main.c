#include "attribute.h"
#include "config.h"
#include "tags.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define CLIB_IMPLEMENTATION
#include "clib.h"


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
        for (size_t i = 0; i <= 6; ++i) {
            Heading(&buffer, i, clib_format_text("Heading %zu", i));
        }
        Paragraph(&buffer, "Hello from C");

    BodyEnd(&buffer);
    HtmlEnd(&buffer);

    Export(buffer, output);
    Clean(&buffer);
    return 0;
}

