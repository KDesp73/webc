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
 * @file webc-actions.h
 * @author KDesp73 (Konstantinos Despoinidis)
 * @version 0.0.1
 */

#ifndef WEBCACTIONS_H
#define WEBCACTIONS_H

#include "webc-core.h"

typedef enum {
    ACTION_EXPORT = 0,
    ACTION_SERVE_STATIC,
    ACTION_SERVE_DYNAMIC,
    ACTION_SERVE_EXPORTED_STATIC,
    ACTION_PRINT_DOCUMENTS
} WebcAction;

typedef struct {
    char* path;
    char* buffer;
} Route;

typedef struct {
    Cstr root;
    Route** routes;
    size_t count;
} Tree;

/**
 * TODO
 */
WEBCAPI Route* MakeRoute(Cstr path, char* buffer);

/**
 * TODO
 */
WEBCAPI Tree MakeTree(Cstr root, Route* first, ...);

/**
 * TODO
 */
WEBCAPI WebcAction ParseCliArgs(int argc, char** argv);

/**
 * TODO
 */
WEBCAPI void HandleAction(WebcAction action, Tree tree);

/**
 * Exports the contents of the buffer to an html file, while also creating the path that is being passed. Finally the buffer is freed
 *
 * @param buffer The contents of the buffer
 * @param path The path to export the html file to
 */
WEBCAPI void Export(char* buffer, Cstr path);

/**
 * Exports the Route struct containing a buffer and a path
 *
 * @param route The Route struct
 */
WEBCAPI void ExportRoute(Route route);

/**
 * TODO
 */
WEBCAPI void ExportTree(Tree tree);

#endif // WEBCACTIONS_H
