#include "webc-templates/markdown.h"
#include "webc-templates/template.h"

#define CLIB_IMPLEMENTATION
#include <webc/extern/clib.h>
#include <webc/webc-core.h>


char* WEBC_Markdown(Template template, const char* file, Theme theme)
{
    char* buffer = NULL;

    WEBC_HtmlStart(&buffer, template.lang);
    WEBC_Head(&buffer, template.title, 
        META_AUTHOR_TAG(template.author),
        NULL
    );

    WEBC_StyleStart(&buffer, NO_ATTRIBUTES);
        if (theme == DARK)
            WEBC_IntegrateFile(&buffer, "https://raw.githubusercontent.com/sindresorhus/github-markdown-css/main/github-markdown-dark.css");
        else 
            WEBC_IntegrateFile(&buffer, "https://raw.githubusercontent.com/sindresorhus/github-markdown-css/main/github-markdown-light.css");
    WEBC_StyleEnd(&buffer);

    Modifier body_mod = {0};
    if (theme == DARK) {
        body_mod.style = strdup("padding: 0; margin: 0; background-color: #0d1117;");
    } else {
        body_mod.style = strdup("padding: 0; margin: 0; background-color: #ffffff;");
    }

    WEBC_BodyStart(&buffer, WEBC_UseModifier(body_mod));
        WEBC_DivStart(&buffer, WEBC_UseModifier((Modifier){.class = "markdown-body", .style = "margin: 5% 25%;"}));
            WEBC_IntegrateFile(&buffer, file);
        WEBC_DivEnd(&buffer);
    WEBC_BodyEnd(&buffer);

    free((char*) body_mod.style);

    WEBC_HtmlEnd(&buffer);

    return buffer;
}
