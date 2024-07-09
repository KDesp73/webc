#include "extern/httpd.h"
#include <pthread.h>

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
    fprintf(stderr, "[%s] %s\n", req.method, req.path);
}

typedef struct {
    server_t server;
    int client_socket;
} thread_data_t;

void* handle_request(void* arg)
{
    thread_data_t* thread_data = (thread_data_t*)arg;
    server_t server = thread_data->server;
    int client_socket = thread_data->client_socket;

    char request_str[MAX_REQUEST_SIZE];
    recv(client_socket, request_str, MAX_REQUEST_SIZE, 0);

    request_t parsed_request = parse_request(request_str);

    // log_reguest(parsed_request); // TODO: on a seperate thread

    response_t* response = server.response_func(parsed_request, server.root);

    if(server.middleware != NULL){
        if(!server.middleware(response, parsed_request)){
            // Log error message
        }
    }

    char* res_str = (char*) response_str(*response);
    clean_response(response);

    send(client_socket, res_str, strlen(res_str), 0);

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

    return server;
}

