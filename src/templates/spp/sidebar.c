#include "webc-templates/spp.h"
#include "webc-templates/template.h"

typedef struct {
    Cstr text;
    Cstr link;
    Cstr target;
} Link;

void WEBC_SidebarLinks(char** buffer)
{
    Link links[] = {
        (Link){.text = "Intro",.link = "#intro", .target = NULL},
        (Link){.text = "Projects",.link = "#projects", .target = NULL},
        (Link){.text = "Skills",.link = "#skills", .target = NULL},
        (Link){.text = "Contact",.link = "#contact", .target = NULL},
    };

    for (size_t i = 0; i < ARRAY_LEN(links); ++i) {
        char* link = (char*) malloc(1); 
        memset(link, 0, 1);
        WEBC_Li(&link, NO_ATTRIBUTES, links[i].text); 
        WEBC_Anchor(buffer, WEBC_UseModifier((Modifier) {.href = links[i].link, .target = links[i].target}), link);
        free(link);
    }
}

void WEBC_SideBar(char** buffer, SinglePagePortfolio portfolio)
{
    WEBC_H2(buffer, NO_ATTRIBUTES, portfolio.github_username);
    WEBC_UlStart(buffer, NO_ATTRIBUTES);
        WEBC_SidebarLinks(buffer);
    WEBC_UlEnd(buffer);
    WEBC_TemplateFooter(buffer, portfolio.author, portfolio.year);
}

