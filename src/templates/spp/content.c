#include "webc-templates/spp.h"
#include "webc-templates/template.h"

#include <string.h>

WEBCAPI void WEBC_Intro(char** buffer, Cstr name, Cstr about)
{
    WEBC_TemplateSection(buffer, "intro");
    
    char* par = clib_format_text("Hello there! My name is %s<br><br>%s", name, about);
    WEBC_Paragraph(buffer, NO_ATTRIBUTES, par);
    free(par);
}

void ShowProject(char** buffer, Project project)
{
    WEBC_DivStart(buffer, WEBC_UseModifier((Modifier) {.class = "project"}));
        char* anchor = (char*) malloc(1);
        memset(anchor, 0, 1);
        WEBC_Anchor(&anchor, WEBC_UseModifier((Modifier) {.href = project.link, .target = "_blank"}), project.title);
        WEBC_H2(buffer, NO_ATTRIBUTES, anchor);
        free(anchor);

        char* subtitle = clib_format_text("<b>Language:</b> %s | <b>License:</b> %s", project.lang, project.license);
        WEBC_Paragraph(buffer, NO_ATTRIBUTES, subtitle);
        free(subtitle);

        WEBC_Paragraph(buffer, NO_ATTRIBUTES, project.desc);

    WEBC_DivEnd(buffer);
}

WEBCAPI void WEBC_Projects(char** buffer, Project projects[], size_t count)
{
    WEBC_TemplateSection(buffer, "projects");

    for(size_t i = 0; i < count; ++i){
        ShowProject(buffer, projects[i]);
    }
    
}

WEBCAPI void WEBC_Skills(char** buffer, Cstr skills[], size_t skills_count)
{
    WEBC_TemplateSection(buffer, "skills");
    WEBC_Paragraph(buffer, NO_ATTRIBUTES, "I am familiar with the following technologies");

    for(size_t i = 0; i < skills_count; i += 6){
        char* skills_line = (char*) malloc(1);
        memset(skills_line, 0, 1);
        for(size_t j = i; j < i+6; ++j){
            char* s = (char*) malloc(1);
            memset(s, 0, 1);
            if(j == 5){
                WEBC_Append(&s, skills[j]);
            } else {
                if(j >= skills_count) {
                    free(s);
                    break;
                }
                char* skill_comma = clib_format_text("%s,", skills[j]);
                WEBC_Append(&s, skill_comma);
                free(skill_comma);
            }

            WEBC_Append(&skills_line, s);
            free(s);
        }

        char* link = clib_format_text("https://skillicons.dev/icons?i=%s", skills_line);
        free(skills_line);
        WEBC_Img(buffer, WEBC_UseModifier((Modifier) {.src = link, .class="skills"}));
        free(link);
    }
}

void Seperator(char** buffer)
{
    WEBC_Br(buffer);
    WEBC_Br(buffer);
    WEBC_Hr(buffer);
    WEBC_Br(buffer);
    WEBC_Br(buffer);
}

void WEBC_Content(char** buffer, SinglePagePortfolio portfolio)
{

    WEBC_Intro(buffer, portfolio.author, portfolio.about);
    Seperator(buffer);
    WEBC_Projects(buffer, portfolio.projects, portfolio.projects_count);
    Seperator(buffer);
    WEBC_Skills(buffer, portfolio.skills, portfolio.skills_count);
    Seperator(buffer);
    WEBC_TemplateContact(buffer, portfolio.email, portfolio.github_username);
}
