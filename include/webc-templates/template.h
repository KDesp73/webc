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
 * @file webc-templates/template.h
 * @author KDesp73 (Konstantinos Despoinidis)
 * @version 0.0.1
 */


#ifndef WEBC_TEMPLATE_H
#define WEBC_TEMPLATE_H
#include "webc-core.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    Cstr name;
    Cstr link;
    Cstr lang;
    Cstr license;
    Cstr version;
    Cstr desc;
    Cstr image;
} Project;

typedef struct {
    Cstr title;
    Cstr author;
    Cstr about;
    Cstr email;
    Cstr github_username;
    int year;
    Cstr style_path;
} Template;

WEBCAPI void WEBC_TemplateFooter(char** buffer, Cstr author, size_t year);
WEBCAPI void WEBC_TemplateContact(char** buffer, Cstr email, Cstr github_username);
WEBCAPI void WEBC_TemplateSection(char** buffer, Cstr title);


#ifdef __cplusplus
}
#endif

#endif // WEBC_TEMPLATE_H

