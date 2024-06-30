#include "webc-templates/spp.h"
#include "webc-core.h"

void fill_portfolio(SinglePagePortfolio* portfolio)
{
    if(portfolio->title == NULL) portfolio->title = "Portfolio";
    if(portfolio->year == 0) portfolio->year = 2024;
    if(portfolio->email == NULL) portfolio->email = "example@email.com";
    if(portfolio->author == NULL) portfolio->author = "John Doe"; 
    if(portfolio->about == NULL) portfolio->about = "";
    if(portfolio->github_username == NULL) portfolio->github_username = "User";
}

char* WEBC_SinglePagePortfolioTemplate(SinglePagePortfolio portfolio)
{
    fill_portfolio(&portfolio);
    char* buffer = NULL;

    WEBC_HtmlStart(&buffer, "en");
        WEBC_Head(&buffer, portfolio.title, 
            META_AUTHOR_TAG(portfolio.author), NULL
        );

        WEBC_StyleStart(&buffer);
        if(portfolio.style_path != NULL)
            WEBC_IntegrateFile(&buffer, portfolio.style_path);
        else
            WEBC_IntegrateFile(&buffer, "https://raw.githubusercontent.com/KDesp73/webc/main/style/spp-style.css");
        WEBC_StyleEnd(&buffer);

        WEBC_BodyStart(&buffer);
            WEBC_DivStart(&buffer, WEBC_UseModifier((Modifier) {.class = "sidebar"}));
                WEBC_SideBar(&buffer, portfolio);
            WEBC_DivEnd(&buffer);

            WEBC_MainStart(&buffer, NO_ATTRIBUTES);
                WEBC_Content(&buffer, portfolio);
            WEBC_MainEnd(&buffer);

        WEBC_BodyEnd(&buffer);

    WEBC_HtmlEnd(&buffer);

    return buffer;
}

