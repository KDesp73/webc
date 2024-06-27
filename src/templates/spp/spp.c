#include "webc-templates/spp.h"

char* SinglePagePortfolioTemplate(SinglePagePortfolio portfolio)
{
    char* buffer = NULL;

    WEBC_HtmlStart(&buffer, "en");
        WEBC_Head(&buffer, portfolio.title,
            WEBC_MakeTag(META, 
                WEBC_MakeAttributeList(
                    WEBC_MakeAttribute(ATTR_NAME, "author"),
                    WEBC_MakeAttribute(ATTR_CONTENT, portfolio.author),
                    NULL
                )
            ),
            NULL
        );

        WEBC_StyleStart(&buffer);
            WEBC_IntegrateFile(&buffer, portfolio.style_path);
        WEBC_StyleEnd(&buffer);

        WEBC_BodyStart(&buffer);
            WEBC_DivStart(&buffer, WEBC_UseModifier((Modifier) {.class = "sidebar"}));
                SideBar(&buffer, portfolio);
            WEBC_DivEnd(&buffer);

            WEBC_MainStart(&buffer, NO_ATTRIBUTES);
                Content(&buffer, portfolio);
            WEBC_MainEnd(&buffer);

        WEBC_BodyEnd(&buffer);

    WEBC_HtmlEnd(&buffer);

    return buffer;
}

