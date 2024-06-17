/*
 * Copyright (c) 2016, Mario Konrad
 * All rights reserved.
 * License: BSD (4 clause), see README.md
 */

#ifndef HTTPD_H
#define HTTPD_H

#ifndef HTTPDAPI
    #define HTTPDAPI static
#endif // HTTPDAPI

#pragma GCC diagnostic ignored "-Wunused-function"

/* ########## Includes ########## */

#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/* ########## Types ########## */

#define UNUSED(arg)  ((void)arg)

struct query_t {
	char val[24];
};

struct header_property_t {
	char key[32];
	char value[128];
};

struct request_t {
	char method[8];
	char protocol[12];
	char url[128];

	/* query data, very constrained regarding memory resources */
	size_t nquery; /* number of queries */
	struct query_t query[8];
	int content_length;
};

struct response_t {
	char head[256];
};

struct server_t {
	int sock;
	struct sockaddr_in addr;

	int (*func_bad_request)(int, const struct request_t *);
	int (*func_request)(int, const struct request_t *);
};

struct client_t {
	int sock;
	struct sockaddr_in addr;
	socklen_t len;
};

static int str_append(char * s, size_t len, char c)
{
	size_t l = strlen(s);
	if (l < len) {
		s[l]= c;
		return 0;
	}
	return -1;
}

/* ########## Declarations ########## */

HTTPDAPI int method_append(struct request_t * r, char c);
HTTPDAPI int protocol_append(struct request_t * r, char c);
HTTPDAPI void request_clear(struct request_t * r);
HTTPDAPI int url_append(struct request_t * r, char c);
HTTPDAPI int query_append(struct request_t * r, char c);
HTTPDAPI int query_next(struct request_t * r);
HTTPDAPI void clear(char * s, size_t len);
HTTPDAPI int append(char * s, size_t len, char c);
HTTPDAPI void clear_header_property(struct header_property_t * prop);
HTTPDAPI int append(char * s, size_t len, char c);
HTTPDAPI int parse(int client_sock, struct request_t * r);
HTTPDAPI int request_bad(int sock, const struct request_t * req);
HTTPDAPI void response_init(struct response_t * res);
HTTPDAPI int response_append_content_type(struct response_t * res, const char * mime);
HTTPDAPI int response_append(struct response_t * res, const char * text, size_t len);
HTTPDAPI int response_append_no_cache(struct response_t * res);
HTTPDAPI int response_append_connection_close(struct response_t * res);
HTTPDAPI int response_append_header_start(struct response_t * res);
HTTPDAPI int response_append_header_end(struct response_t * res);
HTTPDAPI int send_header_mime(int sock, const char * mime);
HTTPDAPI int request_send_file(int sock, const struct request_t * req, const char * filename);
HTTPDAPI int request_response(int sock, const struct request_t * req);
HTTPDAPI void print_req(int rc, struct request_t * r);
HTTPDAPI int run_server(struct server_t * server);

#ifdef HTTPD_IMPLEMENTATION

HTTPDAPI int method_append(struct request_t * r, char c)
{
	return str_append(r->method, sizeof(r->method)-1, c);
}

HTTPDAPI int protocol_append(struct request_t * r, char c)
{
	return str_append(r->protocol, sizeof(r->protocol)-1, c);
}

HTTPDAPI void request_clear(struct request_t * r)
{
	memset(r, 0, sizeof(struct request_t));
}

HTTPDAPI int url_append(struct request_t * r, char c)
{
	return str_append(r->url, sizeof(r->url)-1, c);
}

HTTPDAPI int query_append(struct request_t * r, char c)
{
	if (r->nquery >= sizeof(r->query) / sizeof(struct query_t))
		return -1;
	return str_append(r->query[r->nquery].val, sizeof(r->query[r->nquery].val)-1, c);
}

HTTPDAPI int query_next(struct request_t * r)
{
	if (r->nquery >= sizeof(r->query) / sizeof(struct query_t))
		return -1;
	r->nquery++;
	return 0;
}

HTTPDAPI void clear(char * s, size_t len)
{
	memset(s, 0, len);
}

HTTPDAPI void clear_header_property(struct header_property_t * prop)
{
	clear(prop->key, sizeof(prop->key));
	clear(prop->value, sizeof(prop->value));
}

HTTPDAPI int append(char * s, size_t len, char c)
{
	return str_append(s, len, c);
}

/**
 * Parses received data from \c client_sock and sets corresponding
 * fields of the specified request data structure.
 *
 * \param[in] client_sock Client socket to read data from.
 * \param[out] r The request data structure to fill.
 * \return \c 0 on success, the negative state number in which the
 *   error ocurred.
 */
HTTPDAPI int parse(int client_sock, struct request_t * r)
{
	int state = 0; /* state machine */
	int read_next = 1; /* indicator to read data */
	char c = 0; /* current character */
	char buffer[16]; /* receive buffer */
	int buffer_index = sizeof(buffer); /* index within the buffer */
	int content_length = -1; /* used only in POST requests */
	struct header_property_t prop; /* temporary space to hold header key/value properties*/

	request_clear(r);
	clear_header_property(&prop);
	while (client_sock >= 0) {

		/* read data */
		if (read_next) {

			/* read new data, buffers at a time */
			if (buffer_index >= (int)sizeof(buffer)) {
				int rc;

				memset(buffer, 0, sizeof(buffer));
				rc = read(client_sock, buffer, sizeof(buffer));
				if (rc < 0)
					return -99; /* read error */
				if (rc == 0)
					return 0; /* no data read */
				buffer_index = 0;
			}
			c = buffer[buffer_index];
			++buffer_index;

			/* state management */
			read_next = 0;
		}

		/* execute state machine */
		switch (state) {
			case 0: /* kill leading spaces */
				if (isspace(c)) {
					read_next = 1;
				} else {
					state = 1;
				}
				break;
			case 1: /* method */
				if (isspace(c)) {
					state = 2;
				} else {
					if (method_append(r, c))
						return -state;
					read_next = 1;
				}
				break;
			case 2: /* kill spaces */
				if (isspace(c)) {
					read_next = 1;
				} else {
					state = 3;
				}
				break;
			case 3: /* url */
				if (isspace(c)) {
					state = 5;
				} else if (c == '?') {
					read_next = 1;
					state = 4;
				} else {
					if (url_append(r, c))
						return -state;
					read_next = 1;
				}
				break;
			case 4: /* queries */
				if (isspace(c)) {
					if (query_next(r))
						return -state;
					state = 5;
				} else if (c == '&') {
					if (query_next(r))
						return -state;
					read_next = 1;
				} else {
					if (query_append(r, c))
						return -state;
					read_next = 1;
				}
				break;
			case 5: /* kill spaces */
				if (isspace(c)) {
					read_next = 1;
				} else {
					state = 6;
				}
				break;
			case 6: /* protocol */
				if (isspace(c)) {
					state = 7;
				} else {
					if (protocol_append(r, c))
						return -state;
					read_next = 1;
				}
				break;
			case 7: /* kill spaces */
				if (isspace(c)) {
					read_next = 1;
				} else {
					clear_header_property(&prop);
					state = 8;
				}
				break;
			case 8: /* header line key */
				if (c == ':') {
					state = 9;
					read_next = 1;
				} else {
					if (append(prop.key, sizeof(prop.key)-1, c))
						return -state;
					read_next = 1;
				}
				break;
			case 9: /* kill spaces */
				if (isspace(c)) {
					read_next = 1;
				} else {
					state = 10;
				}
				break;
			case 10: /* header line value */
				if (c == '\r') {
					if (strcmp("Content-Length", prop.key) == 0)
						content_length = strtol(prop.value, 0, 0);
					clear_header_property(&prop);
					state = 11;
					read_next = 1;
				} else {
					if (append(prop.value, sizeof(prop.value)-1, c))
						return -state;
					read_next = 1;
				}
				break;
			case 11:
				if (c == '\n') {
					read_next = 1;
				} else if (c == '\r') {
					state = 12;
					read_next = 1;
				} else {
					state = 8;
				}
				break;
			case 12: /* end of header */
				if (c == '\n') {
					if (content_length > 0) {
						state = 13;
						read_next = 1;
					} else {
						return 0; /* end of header, no content => end of request */
					}
				} else {
					state = 8;
				}
				break;
			case 13: /* content (POST queries) */
				if (c == '&') {
					if (query_next(r))
						return -state;
					read_next = 1;
				} else if (c == '\r') {
					if (query_next(r))
						return -state;
					read_next = 1;
				} else if (c == '\n') {
					read_next = 1;
				} else if (c == '\0') {
					if (query_next(r))
						return -state;
					return 0; /* end of content */
				} else {
					if (query_append(r, c))
						return -state;
					read_next = 1;
				}
				break;
		}
	}
	return -99;
}


HTTPDAPI void print_req(int rc, struct request_t * r)
{
	size_t i;
	if (rc) {
		printf("\nERROR: invalid request: %d", rc);
	} else {
		printf("MET:[%s]\n", r->method);
		printf("PRT:[%s]\n", r->protocol);
		printf("URL:[%s]\n", r->url);
		for (i = 0; i < r->nquery; ++i) printf("QRY:[%s]\n", r->query[i].val);
	}
	printf("\n");
}

HTTPDAPI int run_server(struct server_t * server)
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
			if (server->func_request)
				server->func_request(client.sock, &r);
		} else {
			if (server->func_bad_request)
				server->func_bad_request(client.sock, &r);
		}
		shutdown(client.sock, SHUT_WR);
		close(client.sock);
	}
}

HTTPDAPI int request_bad(int sock, const struct request_t * req)
{
	static const char * RESPONSE =
		"HTTP/1.1 400 Bad Request\r\n"
		"Content-Type: text/html\r\n"
		"Pragma: no-cache\r\n"
		"Cache-Control: no-cache\r\n"
		"Connection: close\r\n"
		"\r\n"
		"<html><body>Bad Request</body></html>\r\n";

	int length = 0;

	UNUSED(req);

	length = strlen(RESPONSE);
	return write(sock, RESPONSE, length) == length ? 0 : -1;
}

HTTPDAPI void response_init(struct response_t * res)
{
	memset(res->head, 0, sizeof(res->head));
}

HTTPDAPI int response_append_content_type(struct response_t * res, const char * mime)
{
	static const char * TEXT = "Content-Type: ";

	if (strlen(res->head) > (sizeof(res->head) - strlen(TEXT) - strlen(mime) - 2))
		return -1;
	strcat(res->head, TEXT);
	strcat(res->head, mime);
	strcat(res->head, "\r\n");
	return 0;
}

HTTPDAPI int response_append(struct response_t * res, const char * text, size_t len)
{
	const size_t n = sizeof(res->head) - strlen(res->head);
	if (len > n)
		return -1;
	strncat(res->head, text, n);
	return 0;
}

HTTPDAPI int response_append_no_cache(struct response_t * res)
{
	static const char * TEXT =
		"Pragma: no-cache\r\n"
		"Cache-Control: no-cache\r\n";
	return response_append(res, TEXT, strlen(TEXT));
}

HTTPDAPI int response_append_connection_close(struct response_t * res)
{
	static const char * TEXT = "Connection: close\r\n";
	return response_append(res, TEXT, strlen(TEXT));
}

HTTPDAPI int response_append_header_start(struct response_t * res)
{
	static const char * TEXT = "HTTP/1.1 200 OK\r\n";
	return response_append(res, TEXT, strlen(TEXT));
}

HTTPDAPI int response_append_header_end(struct response_t * res)
{
	static const char * TEXT = "\r\n";
	return response_append(res, TEXT, strlen(TEXT));
}

HTTPDAPI int send_header_mime(int sock, const char * mime)
{
	int len;
	struct response_t res;

	response_init(&res);
	response_append_header_start(&res);
	response_append_content_type(&res, mime);
	response_append_no_cache(&res);
	response_append_connection_close(&res);
	response_append_header_end(&res);

	len = (int)strlen(res.head);
	return write(sock, res.head, len) == len ? 0 : -1;
}

HTTPDAPI int request_send_file(int sock, const struct request_t * req, const char * filename)
{
	int fd;
	int rc;
	char buf[256];

	UNUSED(req);

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return -1;

	if (send_header_mime(sock, "text/html") >= 0) {
		for (;;) {
			rc = read(fd, buf, sizeof(buf));
			if (rc <= 0) break;
			rc = write(sock, buf, rc);
			if (rc < 0) break;
		}
	}
	close(fd);
	return 0;
}

HTTPDAPI int request_response(int sock, const struct request_t * req)
{
	static const char * RESPONSE =
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/html\r\n"
		"Pragma: no-cache\r\n"
		"Cache-Control: no-cache\r\n"
		"Connection: close\r\n"
		"\r\n"
		"<html><body>Welcome (default response)</body></html>\r\n";

	int length = 0;

	UNUSED(req);

	if (strcmp(req->url, "/") == 0) {
		return request_send_file(sock, req, "index.html");
	} else {
		length = strlen(RESPONSE);
		return (write(sock, RESPONSE, length) == length) ? 0 : -1;
	}
}

#endif // HTTPD_IMPLEMENTATION

#endif // HTTPD_H
