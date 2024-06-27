#ifndef WEBCTEMPLATESPP_H
#define WEBCTEMPLATESPP_H

#include "webc-core.h"

typedef struct {
    Cstr text;
    Cstr link;
    Cstr target;
} Link;

typedef struct {
    Cstr title;
    Cstr link;
    Cstr lang;
    Cstr license;
    Cstr desc;
} Project;

typedef struct {
    Cstr title;
    Cstr author;
    Cstr about;
    Cstr github_username;
    Cstr email;
    int year;
    Cstr style_path;
    Project* projects;
    size_t projects_count;
    Cstr* skills;
    size_t skills_count;
} SinglePagePortfolio;

void SidebarLinks(char** buffer);
void SidebarFooter(char** buffer, Cstr author, size_t year);
void SideBar(char** buffer, SinglePagePortfolio portfolio);

void Intro(char** buffer, Cstr name, Cstr about);
void Projects(char** buffer, Project projects[], size_t count);
void Skills(char** buffer, Cstr skills[], size_t skills_count);
void Contact(char** buffer, Cstr email, Cstr github_username);
void Content(char** buffer, SinglePagePortfolio portfolio);

char* SinglePagePortfolioTemplate(SinglePagePortfolio portfolio);

#endif // WEBCTEMPLATESPP_H
