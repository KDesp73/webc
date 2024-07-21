#define HTTPD_IMPLEMENTATION
#include "extern/httpd.h"
#include "webc-actions.h"
#include "webc-server.h"
#include <assert.h>
#include <string.h>

WEBCAPI int run_server_tree(server_t server, Tree tree)
{
    PANIC("Not implemented yet");
}

int match_url(const char* url, const char* path) {
    return strcmp(url, path) == 0;
}

int find_route(Tree tree, const char* url) {
    char* full_url = url_to_path(url, tree.root);

    for (size_t i = 0; i < tree.count; ++i) {
        Route* route = tree.routes[i];
        char* full_route_path = url_to_path(route->path, tree.root);

        DEBU("full_url: %s", full_url);
        DEBU("full_route_path: %s", full_route_path);

        if (match_url(full_url, full_route_path)) {
            free(full_url);
            free(full_route_path);
            return i;
        }
        free(full_route_path);
    }

    free(full_url);
    return -1; // Not found
}

WEBCAPI response_t* response_tree(request_t request, Tree tree)
{
    PANIC("Not implemented yet");
}

