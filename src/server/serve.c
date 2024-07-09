#include "extern/httpd.h"
#include "webc-actions.h"
#include "webc-server.h"


WEBCAPI int WEBC_ServeExportedRoot(Cstr ip, int port, Cstr root)
{
    DEBU("WEBC_ServeExportedRoot");
    server_t server = server_init(ip, port, root);
	server.response_func = response;

	return run_server(server);
}

WEBCAPI int WEBC_ServeExported(Cstr ip, int port, Tree tree)
{
    DEBU("WEBC_ServeExported");
    server_t server = server_init(ip, port, tree.root);
	server.response_func = response;

	return run_server(server);
}


WEBCAPI int WEBC_ServeTree(Cstr ip, int port, Tree tree)
{
    DEBU("WEBC_ServeTree");
    server_t server = server_init(ip, port, tree.root);
	server.response_func = response;
    
	return run_server_tree(server, tree);
}
