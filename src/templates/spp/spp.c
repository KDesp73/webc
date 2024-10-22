#include "webc-templates/spp.h"
#include "webc-core.h"

void fill_portfolio(SinglePagePortfolio* portfolio)
{
    if(portfolio->template.title == NULL) portfolio->template.title = "Portfolio";
    if(portfolio->template.year == 0) portfolio->template.year = 2024;
    if(portfolio->template.email == NULL) portfolio->template.email = "example@email.com";
    if(portfolio->template.author == NULL) portfolio->template.author = "John Doe"; 
    if(portfolio->template.about == NULL) portfolio->template.about = "";
    if(portfolio->template.github_username == NULL) portfolio->template.github_username = "User";
}

char* WEBC_TemplateSinglePagePortfolio(SinglePagePortfolio portfolio)
{
    fill_portfolio(&portfolio);
    char* buffer = NULL;

    WEBC_HtmlStart(&buffer, "en");
        WEBC_Head(&buffer, portfolio.template.title, 
            META_AUTHOR_TAG(portfolio.template.author), NULL
        );

        WEBC_StyleStart(&buffer, NO_ATTRIBUTES);
        if(portfolio.template.style_path != NULL)
            WEBC_IntegrateFile(&buffer, portfolio.template.style_path);
        else
            WEBC_IntegrateFile(&buffer, "https://raw.githubusercontent.com/KDesp73/webc/main/style/spp-style.css");
        WEBC_StyleEnd(&buffer);

        WEBC_BodyStart(&buffer, NO_ATTRIBUTES);
            WEBC_DivStart(&buffer, WEBC_UseModifier((Modifier) {.class = "sidebar"}));
                WEBC_TemplateSidebar(&buffer, portfolio.template, WEBC_SidebarLinks);
            WEBC_DivEnd(&buffer);

            WEBC_MainStart(&buffer, NO_ATTRIBUTES);
                WEBC_Content(&buffer, portfolio);
            WEBC_MainEnd(&buffer);

        WEBC_BodyEnd(&buffer);

    WEBC_HtmlEnd(&buffer);

    return buffer;
}

