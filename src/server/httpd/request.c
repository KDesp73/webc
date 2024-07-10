#include "extern/httpd.h"

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

int get_request(int socket, request_t* request)
{
    char request_str[MAX_REQUEST_SIZE];
    ssize_t size = recv(socket, request_str, MAX_REQUEST_SIZE, 0);
    
    *request = parse_request(request_str);

    return (size == 0) ? FAILURE : SUCCESS;
}
