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
 * @file webc-md.h
 * @author KDesp73 (Konstantinos Despoinidis)
 * @version 0.0.2
 */

#ifndef WEBCMD_H
#define WEBCMD_H

#include "webc-core.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Converts the content of the markdown file into html
 *
 * @param path The path of the markdown file to convert
 * @return const char* The html
 */
WEBCAPI Cstr WEBC_MarkdownToHtml(Cstr path);

/**
 * Converts the markdown into html
 *
 * @param md The markdown text
 * @return const char* The html
 */
WEBCAPI Cstr WEBC_MarkdownToHtmlText(Cstr md);

#ifdef __cplusplus
}
#endif

#endif // WEBCMD_H
