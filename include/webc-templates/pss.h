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
 * @file webc-templates/pss.h
 * @author KDesp73 (Konstantinos Despoinidis)
 * @version 0.0.1
 */


#ifndef WEBC_TEMPLATE_PSS_H
#define WEBC_TEMPLATE_PSS_H

#include "webc-core.h"
#include "webc-templates/template.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    Cstr title;
    Cstr author;
    Cstr email;
    int year;
    Cstr github_username;
    Project project;
    Cstr style_path;
} ProjectShowcaseSite;

WEBCAPI char* WEBC_TemplateProjectShowcaseSite(ProjectShowcaseSite site);
WEBCAPI void WEBC_TemplateProjectShowcaseSiteFragment(char** buffer, ProjectShowcaseSite site);

#ifdef __cplusplus
}
#endif
#endif // WEBC_TEMPLATE_PSS_H
