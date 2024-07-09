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
 * @file httpd.h
 * @version v0.0.1
 * @author KDesp73 (Konstantinos Despoinidis)
 */

#ifndef HTTPD_H
#define HTTPD_H

#ifndef HTTPDAPI
    #define HTTPDAPI extern
#endif // HTTPDAPI

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wunused-function"
#endif

/* ########## Includes ########## */

#define CLIB_IMPLEMENTATION
#include "clib.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_NAME "httpd server"
#define SUCCESS 0
#define FAILURE 1
#define MAX_REQUEST_SIZE 1024
#define MAX_PATH_LENGTH 256
#define PID_PATH clib_format_text("%s/.local/state/webc-daemon.pid", getenv("HOME"))

/* ########## Types ########## */

typedef char string[256] ;

typedef struct {
    size_t status_code;
    string Content_Type;
    size_t Content_Length;
    string Date;
    string Connection;
    string Cache_Control;
    string Server;
    string ETag;
    string Last_Modified;
} response_header_t;

typedef struct {
    response_header_t header;
    char* content;
} response_t;

typedef struct {
    string Host;
    string User_Agent;
    string Accept;
    string Accept_Encoding;
    string Connection;
    string Cache_Control;
    string Authorization;
    string Cookie;
} request_header_t;

typedef struct {
    request_header_t header;
    string method;
    string path;
} request_t;

typedef struct {
	int socket;
    char* ip;
    int port;
    const char* root;
	struct sockaddr_in addr;

	response_t* (*response_func)(const request_t, const char*);
    int (*middleware)(response_t*, const request_t);
} server_t;

typedef enum {
    DAEMON_START,
    DAEMON_STOP,
    DAEMON_RESTART
} DaemonAction;

/* ########## Declarations ########## */


HTTPDAPI char* url_to_path(const char* url, const char* root);
HTTPDAPI const char* content_type(const char* path);
HTTPDAPI const char* current_date();
HTTPDAPI const char* response_str(response_t response);
HTTPDAPI const char* status_message(size_t status_code);
HTTPDAPI int check_server(server_t server);
HTTPDAPI int has_file_extension(const char *path);
HTTPDAPI int is_index_html_needed(const char *url);
HTTPDAPI int run_daemon(DaemonAction action, server_t server);
HTTPDAPI int run_server(server_t server);
HTTPDAPI request_t parse_request(Cstr request_str);
HTTPDAPI response_header_t header_content(Cstr content, Cstr type, size_t code);
HTTPDAPI response_header_t header_path(Cstr path, size_t code);
HTTPDAPI response_t* response(request_t request, const char* root);
HTTPDAPI server_t server_init(const char* ip, int port, const char* root);
HTTPDAPI void clean_response(response_t* response);

#ifdef HTTPD_IMPLEMENTATION

    // TODO: make HTTPDAPI static / Make library header-only

#endif // HTTPD_IMPLEMENTATION

#endif // HTTPD_H
