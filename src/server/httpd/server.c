#include "extern/httpd.h"

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

void log_reguest(request_t req)
{
    printf("[%s] %s\n", req.method, req.path);
}

HTTPDAPI int run_server(server_t server)
{
    if(check_server(server) == FAILURE) return FAILURE;

    int client_socket;
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    int status = bind(server.socket, (struct sockaddr *)&server.addr, sizeof(server.addr));
    listen(server.socket, 5);

    for(;;) {
        client_socket = accept(server.socket, (struct sockaddr *)&client_addr, &addr_len);

        char request_str[MAX_REQUEST_SIZE];
        recv(client_socket, request_str, MAX_REQUEST_SIZE, 0);

        request_t parsed_request = parse_request(request_str);

        log_reguest(parsed_request);

        response_t response = server.response_func(parsed_request, server.root);

        if(server.middleware != NULL){
            if(!server.middleware(&response, parsed_request)){
                // Log error message
            }
        }

        char* res_str = (char*) response_str(response);
        DEBU("%s", res_str);
        clean_response(&response);

        send(client_socket, res_str, strlen(res_str), 0);

		shutdown(client_socket, SHUT_WR);
        close(client_socket);
    }

    close(server.socket);
    return status;
}
