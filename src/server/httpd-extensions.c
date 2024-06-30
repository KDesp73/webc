#include "webc-actions.h"
#include "webc-server.h"
#include <assert.h>
#include <string.h>

WEBCAPI int run_server_tree(struct server_t * server, Tree tree)
{
	struct client_t client;
	struct request_t r;
	int rc;

	for (;;) {
		client.sock = -1;
		client.len = sizeof(client.addr);

		client.sock = accept(server->sock, (struct sockaddr *)&client.addr, &client.len);
		if (client.sock < 0)
			return -1;
		rc = parse(client.sock, &r);
		print_req(rc, &r);
		if (rc == 0) {
			if (server->func_request.func_request_tree)
				server->func_request.func_request_tree(client.sock, &r, tree);
		} else {
			if (server->func_bad_request)
				server->func_bad_request(client.sock, &r);
		}
        WEBC_CleanTree(tree);
		shutdown(client.sock, SHUT_WR);
		close(client.sock);
	}
}

char* normalize_url(const char* url) {
    size_t len = strlen(url);
    if (len == 0 || (len > 0 && url[len - 1] == '/')) {
        // If URL is empty or ends with a slash, append "index.html"
        char* normalized = (char*)malloc(len + strlen("index.html") + 1);
        if (!normalized) {
            perror("Failed to allocate memory");
            exit(EXIT_FAILURE);
        }
        if (len > 0) {
            strncpy(normalized, url, len);
        }
        strcpy(normalized + len, "index.html");
        return normalized;
    }
    return strdup(url);
}

char* substring(const char* str, size_t start, size_t length) {
    if (str == NULL) {
        return NULL;
    }
    size_t str_len = strlen(str);
    if (start >= str_len) {
        return NULL;
    }

    size_t max_length = str_len - start;
    if (length > max_length) {
        length = max_length;
    }

    char* substr = (char*)malloc(length + 1);
    if (!substr) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    strncpy(substr, str + start, length);
    substr[length] = '\0';

    return substr;
}
char* build_full_url(const char* root, const char* url) {
    assert(url != NULL);
    assert(root != NULL);


    size_t root_len = strlen(root);
    size_t url_len = strlen(url);
    char* n_url = substring(url, 1, url_len);
    url_len = strlen(n_url);
    size_t needs_slash = (root[root_len - 1] != '/');
    size_t full_len = root_len + url_len + needs_slash + 1;

    // Allocate memory for the full URL
    char* full_url = (char*)malloc(full_len);
    if (!full_url) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    // Copy root to full URL
    strcpy(full_url, root);

    // Append '/' if necessary
    if (needs_slash) {
        strcat(full_url, "/");
    }

    // Append the normalized URL
    strcat(full_url, n_url);


    return full_url;
}

int match_url(const char* url, const char* path) {
    return strcmp(url, path) == 0;
}

int find_route(Tree tree, const char* url) {
    char* normalized_url = normalize_url(url);
    if (!normalized_url) {
        perror("Failed to normalize URL");
        exit(EXIT_FAILURE);
    }
    char* full_url = build_full_url(tree.root, normalized_url);
    free(normalized_url); // We don't need normalized_url anymore

    for (size_t i = 0; i < tree.count; ++i) {
        Route* route = tree.routes[i];
        char* full_route_path = build_full_url(tree.root, normalize_url(route->path));

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
WEBCAPI int request_response_tree(int sock, const struct request_t* req, Tree tree) {
    int length = 0;

    UNUSED(req);
    int route_index = find_route(tree, req->url);
    if (route_index == -1) {
        length = strlen(NOT_FOUND_RESPONSE);
        return (write(sock, NOT_FOUND_RESPONSE, length) == length) ? 0 : -1;
    }

    struct response_t res;
    response_init(&res);
    response_append_header_start(&res);
    response_append_content_type(&res, "text/html");
    response_append_connection_keep_alive(&res);
    response_append_header_end(&res);

    length = strlen(res.head);
    if (write(sock, res.head, length) != length) {
        return -1;
    }

    length = strlen(tree.routes[route_index]->buffer);
    return (write(sock, tree.routes[route_index]->buffer, length) == length) ? 0 : -1;
}

