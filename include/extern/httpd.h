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
    #define HTTPDAPI static
#endif // HTTPDAPI

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wunused-function"
#endif

#define CLIB_IMPLEMENTATION
#include "clib.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define SERVER_NAME "httpd server"
#define SUCCESS 0
#define FAILURE 1
#define MAX_REQUEST_SIZE 1024
#define MAX_PATH_LENGTH 256
#define CHUNK_SIZE 1024
#define PID_PATH clib_format_text("%s/.local/state/webc-daemon.pid", getenv("HOME")) // For daemon

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

// DAEMON START
static void start_daemon(server_t server);
static void stop_daemon(void);
static void restart_daemon(server_t server);

HTTPDAPI void daemonize(void) 
{
    pid_t pid;

    // Fork the first time
    pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        // Parent process
        exit(EXIT_SUCCESS);
    }

    // Create a new session
    if (setsid() < 0) {
        perror("setsid failed");
        exit(EXIT_FAILURE);
    }

    // Fork the second time
    pid = fork();
    if (pid < 0) {
        perror("second fork failed");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        // First child process
        exit(EXIT_SUCCESS);
    }

    // Set the file mode creation mask to 0
    umask(0);

    // Close standard file descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Redirect standard file descriptors to /dev/null
    int fd = open("/dev/null", O_RDWR);
    if (fd != STDIN_FILENO) {
        perror("Failed to reopen stdin to /dev/null");
        exit(EXIT_FAILURE);
    }
    if (dup2(fd, STDOUT_FILENO) != STDOUT_FILENO) {
        perror("Failed to reopen stdout to /dev/null");
        exit(EXIT_FAILURE);
    }
    if (dup2(fd, STDERR_FILENO) != STDERR_FILENO) {
        perror("Failed to reopen stderr to /dev/null");
        exit(EXIT_FAILURE);
    }
    if (fd > STDERR_FILENO) {
        if (close(fd) < 0) {
            perror("Failed to close extra file descriptor");
            exit(EXIT_FAILURE);
        }
    }
}

static void start_daemon(server_t server){
    if(clib_file_exists(PID_PATH)){
        char* old_pid = clib_read_file(PID_PATH, "r");
        if(old_pid != NULL){
            free(old_pid);
            WARN("Httpd server daemon is active. Restarting...");
            restart_daemon(server);
        }
        free(old_pid);
    }

    daemonize();

    pid_t pid = getpid();
    char* pid_str = clib_format_text("%d", pid);

    FILE *fptr;
    fptr = fopen(PID_PATH, "w");
    fprintf(fptr, "%s", pid_str);
    fclose(fptr); 
    free(pid_str);

    run_server(server);
}

static void stop_daemon(){
    char* pid_str = clib_read_file(PID_PATH, "r");
    if(pid_str == NULL){
        PANIC("Httpd server daemon is not active");
    }

    INFO("Stopping daemon...");
    char* command = clib_format_text("kill %s", pid_str);
    clib_execute_command(command);
    free(command);
}

static void restart_daemon(server_t server){
    stop_daemon();
    start_daemon(server);
}

HTTPDAPI int run_daemon(DaemonAction action, server_t server)
{

    if(action == DAEMON_START){
        start_daemon(server);
    } else if(action == DAEMON_STOP){
        stop_daemon();
    } else if(action == DAEMON_RESTART){
        restart_daemon(server);
    }

    return action;
}
// DAEMON END

// HEADER START

HTTPDAPI const char* current_date(void)
{
    time_t rawtime;
    struct tm * timeinfo;
    char *buffer = (char*)malloc(80*sizeof(char));

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 80, "%a, %d %b %Y %H:%M:%S GMT", timeinfo);

    return buffer;
}

HTTPDAPI const char* content_type(const char *file_path) 
{
    const char *content_type = "text/plain";

    const char *file_extension = strrchr(file_path, '.');
    if (file_extension) {
        if (strcmp(file_extension, ".html") == 0) {
            content_type = "text/html";
        } else if (strcmp(file_extension, ".css") == 0) {
            content_type = "text/css";
        } else if (strcmp(file_extension, ".js") == 0) {
            content_type = "application/javascript";
        } else if (strcmp(file_extension, ".jpg") == 0 || strcmp(file_extension, ".jpeg") == 0) {
            content_type = "image/jpeg";
        } else if (strcmp(file_extension, ".png") == 0) {
            content_type = "image/png";
        } else if (strcmp(file_extension, ".gif") == 0) {
            content_type = "image/gif";
        }
    }

    return content_type;
}

HTTPDAPI const char* status_message(size_t status_code)
{
    switch(status_code){
        case 101: return "Switching Protocols";
        case 102: return "Processing";
        case 103: return "Early Hints";
        case 200: return "OK";
        case 201: return "Created";
        case 202: return "Accepted";
        case 203: return "Non-Authoritative Information";
        case 204: return "No Content";
        case 205: return "Reset Content";
        case 206: return "Partial Content";
        case 207: return "Multi-Status";
        case 208: return "Already Reported";
        case 226: return "IM Used";
        case 300: return "Multiple Choices";
        case 301: return "Moved Permanently";
        case 302: return "Found";
        case 303: return "See Other";
        case 304: return "Not Modified";
        case 305: return "Use Proxy";
        case 306: return "Switch Proxy";
        case 307: return "Temporary Redirect";
        case 308: return "Permanent Redirect";
        case 400: return "Bad Request";
        case 401: return "Unauthorized";
        case 402: return "Payment Required";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 406: return "Not Acceptable";
        case 407: return "Proxy Authentication Required";
        case 408: return "Request Timeout";
        case 409: return "Conflict";
        case 410: return "Gone";
        case 411: return "Length Required";
        case 412: return "Precondition Failed";
        case 413: return "Payload Too Large";
        case 414: return "URI Too Long";
        case 415: return "Unsupported Media Type";
        case 416: return "Range Not Satisfiable";
        case 417: return "Expectation Failed";
        case 418: return "I'm a teapot";
        case 421: return "Misdirected Request";
        case 422: return "Unprocessable Entity";
        case 423: return "Locked";
        case 424: return "Failed Dependency";
        case 425: return "Too Early";
        case 426: return "Upgrade Required";
        case 428: return "Precondition Required";
        case 429: return "Too Many Requests";
        case 431: return "Request Header Fields Too Large";
        case 451: return "Unavailable For Legal Reasons";
        case 500: return "Internal Server Error";
        case 501: return "Not Implemented";
        case 502: return "Bad Gateway";
        case 503: return "Service Unavailable";
        case 504: return "Gateway Timeout";
        case 505: return "HTTP Version Not Supported";
        case 506: return "Variant Also Negotiates";
        case 507: return "Insufficient Storage";
        case 508: return "Loop Detected";
        case 510: return "Not Extended";
        case 511: return "Network Authentication Required";
        default: break;
    }
    return NULL;
}

static long get_file_size(const char *file_path) 
{
    FILE *file = fopen(file_path, "rb"); // Open the file in binary mode for reading
    if (file == NULL) {
        return -1; // Return -1 if the file cannot be opened
    }

    fseek(file, 0, SEEK_END); // Move the file pointer to the end of the file
    long file_size = ftell(file); // Get the current position of the file pointer, which is the file size

    fclose(file); // Close the file

    return file_size;
}

HTTPDAPI response_header_t header_path(Cstr path, size_t code)
{
    response_header_t header = {
        .status_code = code,
        .Server = SERVER_NAME,
    };
    
    if(clib_file_exists(path))
        header.Content_Length = clib_file_size(path);

    strcpy(header.Date, current_date());
    strcpy(header.Content_Type, content_type(path));

    return header;
}

HTTPDAPI response_header_t header_content(Cstr content, Cstr type, size_t code)
{
    response_header_t header = {
        .status_code = code,
        .Content_Length = 0,
        .Server = SERVER_NAME,
    };

    if(content != NULL)
        header.Content_Length = strlen(content);

    strcpy(header.Date, current_date());
    strcpy(header.Content_Type, (type == NULL) ? "text/html" : type);

    return header;
}


static void append_key(char** buffer, const char* key, const char* value)
{
    if(value == NULL) return;
    if(key == NULL) return;
    if(*buffer == NULL) return;

    char* key_value = clib_format_text("%s: %s\r", key, value);
    if(key_value == NULL) return;
    clib_str_append_ln(buffer, key_value);
    free(key_value);
}

HTTPDAPI char* header_str(response_header_t header)
{
    char* header_str = clib_format_text("HTTP/1.1 %zu %s\r\n", header.status_code, status_message(header.status_code));
    
    append_key(&header_str, "Server", header.Server);
    append_key(&header_str, "Content-Type", header.Content_Type);
    char* length = clib_format_text("%zu", header.Content_Length);
    if(length != NULL){
        append_key(&header_str, "Content-Length", length);
        free(length);
    }
    append_key(&header_str, "Connection", header.Connection);
    append_key(&header_str, "Date", header.Date);
    clib_str_append(&header_str, "\r\n");

    return header_str;
}

// HEADER END

// REQUEST START

HTTPDAPI request_t parse_request(Cstr request_str)
{
    request_t parsed_request;
    sscanf(request_str, "%s %s HTTP/1.1", parsed_request.method, parsed_request.path);
    sscanf(request_str, "Host: %s", parsed_request.header.Host);
    sscanf(request_str, "User-Agent: %s", parsed_request.header.User_Agent);
    sscanf(request_str, "Accept: %s", parsed_request.header.Accept);
    sscanf(request_str, "Accept-Encoding: %s", parsed_request.header.Accept_Encoding);
    sscanf(request_str, "Connection: %s", parsed_request.header.Connection);
    sscanf(request_str, "Cache-Control: %s", parsed_request.header.Cache_Control);
    sscanf(request_str, "Authorization: %s", parsed_request.header.Authorization);
    sscanf(request_str, "Cookie: %s", parsed_request.header.Cookie);

    return parsed_request;
}

// REQUEST END

// RESPONSE START

HTTPDAPI const char* response_str(response_t response)
{
    char* response_str = header_str(response.header);

    for(size_t i = 0; i < response.chunks_count; ++i){
        clib_str_append_ln(&response_str, response.chunks[i]);
    }

    return response_str;
}

// TODO: Should not use the webc library
static char* ErrorPage(size_t code)
{
    return NULL; // TODO
}

HTTPDAPI void clean_response(response_t* response)
{
    for(size_t i = 0; i < response->chunks_count; ++i){
        if(response->chunks[i] != NULL)
            free(response->chunks[i]);
    }
    free(response->chunk_sizes);
    free(response->chunks);
    free(response);
}

static int is_image(const char *file_path) 
{
    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        return 0;
    }

    unsigned char header[4];
    fread(header, 1, sizeof(header), file);
    fclose(file);

    if (header[0] == 0xFF && header[1] == 0xD8 && header[2] == 0xFF) {
        return 1; // JPEG/JFIF file
    } else if (header[0] == 0x89 && header[1] == 0x50 && header[2] == 0x4E && header[3] == 0x47) {
        return 1; // PNG file
    } else if (header[0] == 0x47 && header[1] == 0x49 && header[2] == 0x46 && header[3] == 0x38) {
        return 1; // GIF file
    } else if (header[0] == 0x42 && header[1] == 0x4D) {
        return 1; // BMP file
    }

    return 0;
}


HTTPDAPI void make_chunks(response_t* response, size_t count)
{
    response->chunks = (char**) malloc(count * sizeof(char*));
    response->chunk_sizes = (size_t*)malloc(count * sizeof(size_t));
    response->chunks_count = count;

    // 0-init
    for(size_t i = 0; i < count; ++i){
        response->chunks[i] = NULL;
        response->chunk_sizes[i] = 0;
    }
}

HTTPDAPI void read_content(response_t* response, const char* path) 
{
    FILE* file = fopen(path, "rb");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    size_t num_chunks = (file_size + CHUNK_SIZE - 1) / CHUNK_SIZE;
    make_chunks(response, num_chunks);

    for (size_t i = 0; i < num_chunks; i++) {
        response->chunks[i] = (char*)malloc(CHUNK_SIZE);
        response->chunk_sizes[i] = fread(response->chunks[i], 1, CHUNK_SIZE, file);
    }

    // Check if the total bytes read match the file size
    size_t total_bytes_read = 0;
    for (size_t i = 0; i < num_chunks; i++) {
        total_bytes_read += response->chunk_sizes[i];
    }

    if (total_bytes_read < file_size) {
        size_t remaining_bytes = file_size - total_bytes_read;
        response->chunks[num_chunks - 1] = (char*)realloc(response->chunks[num_chunks - 1], remaining_bytes);
        size_t bytes_read_last_chunk = fread(response->chunks[num_chunks - 1], 1, remaining_bytes, file);
        response->chunk_sizes[num_chunks - 1] = bytes_read_last_chunk;
    }

    fclose(file);
}


HTTPDAPI response_t* new_response(Cstr path, Cstr content, Cstr type, size_t code)
{
    if(path == NULL){
        if(content == NULL || type == NULL){
            PANIC("When path is NULL, content and type must be set");
        }
    }

    if(content == NULL || type == NULL){
        if(path == NULL){
            PANIC("When content and/or type are NULL, path must be set");
        }
    }

    response_t* response = malloc(sizeof(response_t));
    
    if(path == NULL){
        response->header = header_content(content, type, code);
        make_chunks(response, 1);
        response->chunks[0] = (char*) malloc(strlen(content)+1);
        response->chunk_sizes[0] = strlen(content)+1;
        strcpy(response->chunks[0], (char*) content);
    } else {
        response->header = header_path(path, code);
        read_content(response, path);
        strcpy(response->header.Connection, "keep-alive");
    }
    
    return response;
}

HTTPDAPI response_t* error_response(size_t code)
{
    char* error_page = ErrorPage(code);
    if(error_page == NULL) {
        error_page = clib_format_text("<html><body><h1>%zu %s</h1></body></html>", code, status_message(code)); // fallback html
    }

    response_t* response = new_response(NULL, error_page, "text/html", code);
    free(error_page);
    return response;
}

HTTPDAPI response_t* response(request_t request, const char* root)
{
    char* path = url_to_path(request.path, root);

    response_t* response = NULL;

    if(!clib_file_exists(path)){
        response = error_response(404);
    } else {
        response = new_response(path, NULL, NULL, 200);
    }
    free(path);

    if(response == NULL){
        response = error_response(500); // Internal Server Error
    }

    return response;
}

// RESPONSE END

// SERVER START

HTTPDAPI int check_server(server_t server)
{
    if(server.response_func == NULL) {
        ERRO("Server response function is NULL");
        return FAILURE;
    }

    if(server.root == NULL){
        ERRO("Server root is NULL");
        return FAILURE;
    }

    // TODO: more checks

    return SUCCESS;
}

HTTPDAPI void log_reguest(request_t req)
{
    fprintf(stderr, "[%s] %s\n", req.method, req.path);
}

HTTPDAPI void log_response(response_t res)
{
    fprintf(stderr, " -> %zu\n", res.header.status_code);
}

typedef struct {
    server_t server;
    int client_socket;
} thread_data_t;


HTTPDAPI void send_response(response_t* response, int client_socket)
{
    char* header = header_str(response->header);
    send(client_socket, header, strlen(header), 0);
    free(header);

    for(size_t i = 0; i < response->chunks_count; ++i){
        if(response->chunks[i] == NULL) continue;

        size_t bytes_sent = send(client_socket, response->chunks[i], response->chunk_sizes[i], 0);
        if(bytes_sent != response->chunk_sizes[i]){
            ERRO("Sent %zu bytes of chunk[%zu]'s %zu", bytes_sent, i, response->chunk_sizes[i]);
        }
            
    }
}

static void* handle_request(void* arg)
{
    thread_data_t* thread_data = (thread_data_t*)arg;
    server_t server = thread_data->server;
    int client_socket = thread_data->client_socket;

    char request_str[MAX_REQUEST_SIZE];
    recv(client_socket, request_str, MAX_REQUEST_SIZE, 0);

    request_t parsed_request = parse_request(request_str);

    log_reguest(parsed_request); // TODO: on a seperate thread

    response_t* response = server.response_func(parsed_request, server.root);

    if(server.middleware != NULL){
        if(!server.middleware(response, parsed_request)){
            // Log error message
        }
    }

    send_response(response, client_socket);
    log_response(*response);
    free(response);

    shutdown(client_socket, SHUT_WR);
    close(client_socket);

    return 0;
}

HTTPDAPI int run_server(server_t server)
{
    if(check_server(server) == FAILURE) return FAILURE;

    int client_socket;
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    int status = bind(server.socket, (struct sockaddr *)&server.addr, sizeof(server.addr));
    listen(server.socket, 5);

    thread_data_t* data = (thread_data_t*)malloc(sizeof(thread_data_t));
    data->server = server;
    for(;;) {
        client_socket = accept(server.socket, (struct sockaddr *)&client_addr, &addr_len);

        data->client_socket = client_socket;

        pthread_t tid;
        pthread_create(&tid, NULL, handle_request, data);
        pthread_detach(tid);
    }
    free(data);

    close(server.socket);
    return status;
}

HTTPDAPI server_t server_init(const char* ip, int port, const char* root)
{
    server_t server;
    const int reuse = 1;

    memset(&server, 0, sizeof(server));
    server.socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server.socket < 0) {
        perror("socket");
        exit(1);
    }

    if (ip == NULL) {
        server.ip = strdup("127.0.0.1"); // Allocate memory and copy the string
    } else {
        server.ip = strdup(ip); // Allocate memory and copy the string
    }

    server.root = root;

    memset(&server.addr, 0, sizeof(server.addr));
    server.addr.sin_family = AF_INET;
    server.addr.sin_port = htons(port);
    server.addr.sin_addr.s_addr = htonl(INADDR_ANY);
    inet_pton(AF_INET, server.ip, &server.addr.sin_addr);

    if (bind(server.socket, (const struct sockaddr *)&server.addr, sizeof(server.addr)) < 0) {
        perror("bind");
        exit(1);
    }

    if (setsockopt(server.socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt");
        exit(1);
    }

    if (listen(server.socket, 2) < 0) {
        perror("listen");
        exit(1);
    }

    server.middleware = NULL;

    // Add more response methods to server
    
    server.response_func = response;

    return server;
}

// SERVER END

// UTILS START

HTTPDAPI int has_file_extension(const char *path) 
{
    const char *extension = strrchr(path, '.');
    
    if (extension != NULL) {
        if (*(extension + 1) != '\0') {
            return true;
        }
    }
    
    return false;
} 

HTTPDAPI int is_index_html_needed(const char *url) 
{
    size_t url_len = strlen(url);
    if (url_len == 0 || url[url_len - 1] == '/' || !has_file_extension(url)) {
        return true;
    } else {
        return false;
    }
}

HTTPDAPI char* url_to_path(const char* url, const char* root)
{ 
    char* path = NULL;
    size_t url_len = strlen(url);
    if(is_index_html_needed(url)) {
        if(url[url_len - 1] == '/'){
            path = clib_format_text("%s%sindex.html", root, url);
        } else {
            if(url[0] == '/')
                path = clib_format_text("%s/%s/index.html", root, url);
            else 
                path = clib_format_text("%s%s/index.html", root, url);
        }
    } else {
        path = clib_format_text("%s%s", root, url);
    }

    return path;
}

// UTILS END

#endif // HTTPD_IMPLEMENTATION

#endif // HTTPD_H
