#include "extern/httpd.h"
#include <string.h>
#include <time.h>


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
long get_file_size(const char *file_path) 
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
        .Server = SERVER_NAME,
        .Content_Length = 0,
    };

    if(content != NULL)
        header.Content_Length = strlen(content);

    strcpy(header.Date, current_date());
    strcpy(header.Content_Type, (type == NULL) ? "text/html" : type);

    return header;
}


void append_key(char** buffer, const char* key, const char* value)
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
