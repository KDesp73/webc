/**
 * MIT License
 * 
 * Copyright (c) 2024 Konstantinos Despoinidis 
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file webc-templates/spp.h
 * @author KDesp73 (Konstantinos Despoinidis)
 * @version 0.0.1
 */


#ifndef WEBCTEMPLATESPP_H
#define WEBCTEMPLATESPP_H

#include "webc-core.h"

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

WEBCAPI void WEBC_SidebarLinks(char** buffer);
WEBCAPI void WEBC_SidebarFooter(char** buffer, Cstr author, size_t year);
WEBCAPI void WEBC_SideBar(char** buffer, SinglePagePortfolio portfolio);
WEBCAPI void WEBC_Intro(char** buffer, Cstr name, Cstr about);
WEBCAPI void WEBC_Projects(char** buffer, Project projects[], size_t count);
WEBCAPI void WEBC_Skills(char** buffer, Cstr skills[], size_t skills_count);
WEBCAPI void WEBC_Contact(char** buffer, Cstr email, Cstr github_username);
WEBCAPI void WEBC_Content(char** buffer, SinglePagePortfolio portfolio);
WEBCAPI char* WEBC_SinglePagePortfolioTemplate(SinglePagePortfolio portfolio);

#endif // WEBCTEMPLATESPP_H
