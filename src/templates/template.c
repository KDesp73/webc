#include "webc-templates/template.h"
#include <ctype.h>

WEBCAPI void WEBC_TemplateFooter(char** buffer, Cstr author, size_t year)
{
    WEBC_Paragraph(buffer, NO_ATTRIBUTES, "Made with <a href=\"https://github.com/KDesp73/webc\" target=\"_blank\">webc</a>");
    char* copyright = clib_format_text("Copyright (c) %s %zu. Some rights reserved",author, year);
    WEBC_Paragraph(buffer, NO_ATTRIBUTES, copyright);
    free(copyright);
}

void WayOfContact(char** buffer, Cstr label, Cstr title, Cstr link)
{
    char* anchor = (char*) malloc(1);
    memset(anchor,0,1);
    WEBC_Anchor(&anchor, WEBC_UseModifier((Modifier) {.href = link}), title);
    char* paragraph = clib_format_text("%s: %s", label, anchor);
    free(anchor);
    WEBC_Paragraph(buffer, NO_ATTRIBUTES, paragraph);
    free(paragraph);
}

WEBCAPI void WEBC_TemplateContact(char** buffer, Cstr email, Cstr github_username)
{
    WEBC_TemplateSection(buffer, "contact");

    char* email_link = clib_format_text("mailto:%s", email);
    WayOfContact(buffer, "Email", email, email_link);
    free(email_link);

    char* github_link = clib_format_text("https://github.com/%s", github_username);
    char* github_tag = clib_format_text("@%s", github_username);
    WayOfContact(buffer, "Github", github_tag, github_link);
    free(github_link);
    free(github_tag);
}

char* capitalize_first_letter(const char *str) {
    if (str != NULL && strlen(str) > 0) {
        char *copy = malloc(strlen(str) + 1);
        strcpy(copy, str);
        copy[0] = toupper(copy[0]);
        return copy;
    }
    return NULL;
}

void Section(char** buffer, Cstr title)
{
    Modifier m = {
        .id = title
    };
    char* t = capitalize_first_letter(title);
    WEBC_H1(buffer, WEBC_UseModifier(m), t);
    free(t);
    WEBC_Br(buffer);
}
