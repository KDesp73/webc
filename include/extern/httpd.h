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
 * @version v0.0.4
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
#define CHUNK_SIZE 1024

// For daemon (needs freeing)
#define PID_PATH clib_format_text("%s/.local/state/webc-daemon.pid", getenv("HOME"))

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
    char** chunks;
    size_t* chunk_sizes;
    size_t chunks_count;
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

/**
 * Translates url to local path starting from root
 *
 * /contact -> <root>/contact/index.html
 *
 * @param url The url taken from the http request
 * @param root The path that is considered the root of the website
 * @return char* The final path
 */
HTTPDAPI char* url_to_path(const char* url, const char* root);

/**
 * Takes a file path and returns its MIME type
 *
 * @param path The file path
 * @return const char* The MIME type
 */
HTTPDAPI const char* content_type(const char* path);

/**
 * Returns the current date in the HTTP format
 *
 * @return const char* The current date
 */
HTTPDAPI const char* current_date(void);

/**
 * Formats the response struct so that it is ready to be sent to the browser
 *
 * @param response The response struct
 * @return const char* The response as a string
 */
HTTPDAPI const char* response_str(response_t response);

/**
 * Returns the status message for each HTTP status code
 *
 * @param status_code The status code [101,511]
 * @return const char* The status message
 */
HTTPDAPI const char* status_message(size_t status_code);


/**
 * Checks if the server_t struct is ready to use
 *
 * 1. The root must be set
 *
 * 2. The request_func function pointer must be set
 *
 * TODO: more checks
 *
 * @param server The server struct
 * @return int True when everything is OK
 */
HTTPDAPI int check_server(server_t server);

/**
 * Checks if the path is a file
 *
 * @param path The path to check
 * @return int True if path is a file
 */
HTTPDAPI int has_file_extension(const char *path);

/**
 * Checks if url implies that index.html is being requested
 *
 * Example: /contact -> True
 *
 *          /contact/index.html -> False
 *
 * @param url The url taken from the HTTP request
 * @return int True if it is needed
 */
HTTPDAPI int is_index_html_needed(const char *url);

/**
 * Runs the server as a daemon
 *
 * @param action The DaemonAction enum (DAEMON_START, DAEMON_STOP, DAEMON_RESTART)
 * @param server The server struct
 * @return int Status code
 */
HTTPDAPI int run_daemon(DaemonAction action, server_t server);

/**
 * Runs the server
 *
 * @param server The server struct
 * @return int Status code
 */
HTTPDAPI int run_server(server_t server);

/**
 * Parses the HTTP request received into a request_t struct for easier usage
 *
 * @param request_str The HTTP request
 * @return request_t The parsed request_t struct
 */
HTTPDAPI request_t parse_request(Cstr request_str);

/**
 * Makes a response_header_t struct from the content itself
 *
 * @param content The content to send
 * @param type The MIME type of the content
 * @param code The HTTP status code
 * @return response_header_t The header struct
 */
HTTPDAPI response_header_t header_content(Cstr content, Cstr type, size_t code);

/**
 * Makes a response_header_t struct from the file path
 *
 * @param path The file path
 * @param code The HTTP status code
 * @return response_header_t The header struct
 */
HTTPDAPI response_header_t header_path(Cstr path, size_t code);

/**
 * Determines the appropriate response to the HTTP request
 *
 * @param request The request_t struct
 * @param root The website root path
 * @return response_t* The response_t struct pointer containing the HTTP response
 */
HTTPDAPI response_t* response(request_t request, const char* root);

/**
 * Initializes a server_t struct (should be used)
 * 
 * @param ip The ip to serve to (127.0.0.1 when NULL)
 * @param port The port to serve to 
 * @param root The site's root
 * @return server_t The initialized server struct
 */
HTTPDAPI server_t server_init(const char* ip, int port, const char* root);

/**
 * Frees the memory allocated for a response struct
 *
 * @param response The response_t struct pointer
 */
HTTPDAPI void clean_response(response_t* response);

/**
 * Forks to a new process and closes all standard descriptors
 */
HTTPDAPI void daemonize(void); // 😈

/**
 * Returns a response_t struct pointer containing a templated Error Page
 *
 * @param code The HTTP status code
 * @return response_t* The response struct pointer
 */
HTTPDAPI response_t* error_response(size_t code);

/**
 * Sends the response_t struct pointer to the client_socket
 *
 * @param response The response_t struct pointer
 * @param client_socket The client's socket 
 */
HTTPDAPI void send_response(response_t* response, int client_socket);

/**
 * Formats the response_header_t struct according to the HTTP protocol
 *
 * @param header The response_header_t struct
 * @return char* The header as a string
 */
HTTPDAPI char* header_str(response_header_t header);

/**
 * Reads the content of the file into 1KB chunks
 *
 * @param response The response_t struct pointer to put the chunks
 * @param path The file to read
 */
HTTPDAPI void read_content(response_t* response, const char* path);

/**
 * Allocates memory for the chunk and chunk_sizes arrays and 0-inits their items
 *
 * @param response The response_t struct pointer
 * @param count The number of chunks to allocate for
 */
HTTPDAPI void make_chunks(response_t* response, size_t count);

/**
 * Logs the request_t struct to stderr
 *
 * @param req The request_t struct to log
 */
HTTPDAPI void log_reguest(request_t req);

/**
 * Logs the response_t struct to stderr
 *
 * @param res The response_t struct to log
 */
HTTPDAPI void log_response(response_t res);


#ifdef HTTPD_IMPLEMENTATION

// TODO: make HTTPDAPI static / Make library header-only

#endif // HTTPD_IMPLEMENTATION

#endif // HTTPD_H
