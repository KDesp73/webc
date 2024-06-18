#include "webc-actions.h"
#include "webc-server.h"

struct server_t setup(int port)
{
	static struct server_t server;
	const int reuse = 1;

	memset(&server, 0, sizeof(server));
	server.sock = -1;

	server.sock = socket(AF_INET, SOCK_STREAM, 0);
	if (server.sock < 0) {
		perror("socket");
		exit(1);
	}

	memset(&server.addr, 0, sizeof(server.addr));
	server.addr.sin_family = AF_INET;
	server.addr.sin_port = htons(port);
	server.addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(server.sock, (const struct sockaddr *)&server.addr, sizeof(server.addr)) < 0) {
		perror("bind");
		exit(1);
	}

	if (setsockopt(server.sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
		perror("setsockopt");
		exit(1);
	}

	if (listen(server.sock, 2) < 0) {
		perror("listen");
		exit(1);
	}

	server.func_bad_request = request_bad;

    return server;
}

WEBCAPI int ServeExportedRoot(int port, Cstr root)
{
    struct server_t server = setup(port);
	server.func_request.func_request_root= request_response;

	return run_server(&server, root);
}

WEBCAPI int ServeExported(int port, Tree tree)
{
    struct server_t server = setup(port);
	server.func_request.func_request_root= request_response;

	return run_server(&server, tree.root);
}


WEBCAPI int ServeTree(int port, Tree tree)
{
    struct server_t server = setup(port);
	server.func_request.func_request_tree = request_response_tree;
    
	return run_server_tree(&server, tree);
}
