/*
 * Copyright (c) 2016, Mario Konrad
 * All rights reserved.
 * License: BSD (4 clause), see README.md
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

#define SERVER_NAME "WEBC HTTP Server"
#define SUCCESS 0
#define FAILURE 1
#define MAX_REQUEST_SIZE 1024
#define MAX_PATH_LENGTH 256

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

	response_t (*response_func)(const request_t, const char*);
    int (*middleware)(response_t*, const request_t);
} server_t;

/* ########## Declarations ########## */

HTTPDAPI int has_file_extension(const char *path);
HTTPDAPI int is_index_html_needed(const char *url);

HTTPDAPI const char* current_date();
HTTPDAPI const char* response_str(response_t response);
HTTPDAPI response_t response(request_t request, const char* root);
HTTPDAPI const char* status_message(size_t status_code);
HTTPDAPI const char* content_type(const char* path);
HTTPDAPI request_t parse_request(Cstr request_str);
HTTPDAPI int run_server(server_t server);
HTTPDAPI int check_server(server_t server);
HTTPDAPI response_header_t header_content(Cstr content, Cstr type, size_t code);
HTTPDAPI response_header_t header_path(Cstr path, size_t code);
HTTPDAPI char* url_to_path(const char* url, const char* root);
HTTPDAPI void clean_response(response_t* response);
HTTPDAPI server_t server_init(const char* ip, int port, const char* root);

#ifdef HTTPD_IMPLEMENTATION

    // TODO: make HTTPDAPI static / Make library header-only

#endif // HTTPD_IMPLEMENTATION

#endif // HTTPD_H
