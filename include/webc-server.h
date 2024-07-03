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
 * @file webc-server.h
 * @author KDesp73 (Konstantinos Despoinidis)
 * @version 0.0.1
 */

#ifndef WEBCSERVER_H
#define WEBCSERVER_H

#include "webc-actions.h"
#include "webc-core.h"
#define HTTPD_IMPLEMENTATION
#include "extern/httpd.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Serves the physical file tree exported from this program starting from the root
 *
 * @param port The port to start the server from
 * @param tree The virtual tree
 *
 * @return int Success code
 */
WEBCAPI int WEBC_ServeExported(int port, Tree tree);

/**
 * Serves a physical file tree starting from the root
 *
 * @param port The port to start the server from
 * @param root The root directory to start serving from
 *
 * @return int Success code
 */
WEBCAPI int WEBC_ServeExportedRoot(int port, Cstr root);

/**
 * Serves a the virtual tree created by the user without exporting it
 *
 * @param port The port to start the server from
 * @param root The root directory
 *
 * @return int Success code
 */
WEBCAPI int WEBC_ServeTree(int port, Tree tree);

// HTTPD Extensions
int request_response_tree(int sock, const struct request_t* req, Tree tree);
int run_server_tree(struct server_t * server, Tree tree);

#ifdef __cplusplus
}
#endif

#endif // WEBCSERVER_H

