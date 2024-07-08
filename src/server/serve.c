#include "extern/httpd.h"
#include "webc-actions.h"
#include "webc-server.h"

server_t setup(int port, Cstr root)
{
	static server_t server;
	const int reuse = 1;

	memset(&server, 0, sizeof(server));
	server.socket = -1;

	server.socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server.socket < 0) {
		perror("socket");
		exit(1);
	}

    server.root = root;

	memset(&server.addr, 0, sizeof(server.addr));
	server.addr.sin_family = AF_INET;
	server.addr.sin_port = htons(port);
	server.addr.sin_addr.s_addr = htonl(INADDR_ANY);
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

WEBCAPI int WEBC_ServeExportedRoot(int port, Cstr root)
{
    server_t server = setup(port, root);
	server.response_func = response;

	return run_server(server);
}

WEBCAPI int WEBC_ServeExported(int port, Tree tree)
{
    server_t server = setup(port, tree.root);
	server.response_func = response;

	return run_server(server);
}


WEBCAPI int WEBC_ServeTree(int port, Tree tree)
{
    server_t server = setup(port, tree.root);
	server.response_func = response;
    
	return run_server_tree(server, tree);
}
