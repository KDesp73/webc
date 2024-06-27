#include "webc-templates/spp.h"

void SidebarLinks(char** buffer)
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

void SidebarFooter(char** buffer, Cstr author, size_t year)
{
    WEBC_Paragraph(buffer, NO_ATTRIBUTES, "Made with <a href=\"https://github.com/KDesp73/webc\" target=\"_blank\">webc</a>");
    char* copyright = clib_format_text("Copyright (c) %s %zu",author, year);
    WEBC_Paragraph(buffer, NO_ATTRIBUTES, copyright);
    free(copyright);
}

void SideBar(char** buffer, SinglePagePortfolio portfolio)
{
    WEBC_H2(buffer, NO_ATTRIBUTES, portfolio.github_username);
    WEBC_UlStart(buffer, NO_ATTRIBUTES);
        SidebarLinks(buffer);
    WEBC_UlEnd(buffer);
    WEBC_FooterStart(buffer, NO_ATTRIBUTES);
        SidebarFooter(buffer, portfolio.author, portfolio.year);
    WEBC_FooterEnd(buffer);
}

