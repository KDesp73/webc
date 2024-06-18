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
 * @version 0.0.2
 */

#ifndef WEBCACTIONS_H
#define WEBCACTIONS_H

#define CLIB_IMPLEMENTATION
#include "extern/clib.h"
#include "webc-core.h"

typedef struct {
    Bool export;
    Bool serve_static;
    Bool serve_dynamic;
    Bool serve_exported_static;
    int port;
    char* root;
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
 * Creates a Route struct pointer
 *
 * @param path The path of the route (physical or virtual)
 * @param buffer The html to serve or export
 *
 * @return Route*
 */
WEBCAPI Route* MakeRoute(Cstr path, char* buffer);

/**
 * Creates a Tree struct contatining multiple Route struct pointers
 *
 * @param root The root of the tree (in case of serving without exporting it doesn't really matter)
 * @param first The first Route struct pointer to add 
 * @param ... The rest of the Route pointers to add
 *
 * @return Tree
 */
WEBCAPI Tree MakeTree(Cstr root, Route* first, ...);
#define CleanRoute(route) free(route->buffer);
#define CleanTree(tree) \
    do {\
        for(size_t i = 0; i < tree.count; ++i){ \
            CleanRoute(tree.routes[i]); \
        } \
    } while(0);

/**
 * Parses the cli arguments
 *
 * @param argc
 * @param argv
 *
 * @return WebcAction struct
 */
WEBCAPI WebcAction ParseCliArgs(int argc, char** argv);

/**
 * Depending on the action the method exports / serves the tree (physical or virtual)
 *
 * @param action the WebcAction returned from the ParseCliArgs method
 * @param tree The virtual tree created by the user
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
 * Exports every Route in the tree
 *
 * @param tree The virtual tree created by the user
 */
WEBCAPI void ExportTree(Tree tree);

#endif // WEBCACTIONS_H
