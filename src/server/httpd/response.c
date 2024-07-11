#include "extern/httpd.h"
#include "webc-core.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


HTTPDAPI const char* response_str(response_t response)
{
    char* response_str = header_str(response.header);

    for(size_t i = 0; i < response.chunks_count; ++i){
        clib_str_append_ln(&response_str, response.chunks[i]);
    }

    return response_str;
}

// TODO: Should not use the webc library
char* ErrorPage(size_t code)
{
    char* buffer = NULL;

    WEBC_HtmlStart(&buffer, "en");
        char* css =
        "body {"
            "font-family: Arial, sans-serif;"
            "background-color: #f0f0f0;"
            "color: #333;"
            "text-align: center;"
            "padding: 50px;"
        "}"
        ""
        "h2 {"
            "font-size: 2em;"
            "color: #333;"
            "margin-bottom: 20px;"
        "}";
        WEBC_StyleStart(&buffer);
            WEBC_PlainText(&buffer, css);
        WEBC_StyleEnd(&buffer);
        WEBC_BodyStart(&buffer);
            char* text = clib_format_text("%zu %s", code, status_message(code));
            WEBC_H2(&buffer, NO_ATTRIBUTES, text);
            free(text);
        WEBC_BodyEnd(&buffer);
    WEBC_HtmlEnd(&buffer);

    return buffer;
}

HTTPDAPI void clean_response(response_t* response)
{
    for(size_t i = 0; i < response->chunks_count; ++i){
        if(response->chunks[i] != NULL)
            free(response->chunks[i]);
    }
    free(response->chunk_sizes);
    free(response->chunks);
    free(response);
}

int is_image(const char *file_path) 
{
    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        return 0;
    }

    unsigned char header[4];
    fread(header, 1, sizeof(header), file);
    fclose(file);

    if (header[0] == 0xFF && header[1] == 0xD8 && header[2] == 0xFF) {
        return 1; // JPEG/JFIF file
    } else if (header[0] == 0x89 && header[1] == 0x50 && header[2] == 0x4E && header[3] == 0x47) {
        return 1; // PNG file
    } else if (header[0] == 0x47 && header[1] == 0x49 && header[2] == 0x46 && header[3] == 0x38) {
        return 1; // GIF file
    } else if (header[0] == 0x42 && header[1] == 0x4D) {
        return 1; // BMP file
    }

    return 0;
}


void make_chunks(response_t* response, size_t count)
{
    response->chunks = (char**) malloc(count * sizeof(char*));
    response->chunk_sizes = (size_t*)malloc(count * sizeof(size_t));
    response->chunks_count = count;

    // 0-init
    for(size_t i = 0; i < count; ++i){
        response->chunks[i] = NULL;
        response->chunk_sizes[i] = 0;
    }
}

HTTPDAPI void read_content(response_t* response, const char* path) 
{
    FILE* file = fopen(path, "rb");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    size_t num_chunks = (file_size + CHUNK_SIZE - 1) / CHUNK_SIZE;
    make_chunks(response, num_chunks);

    for (size_t i = 0; i < num_chunks; i++) {
        response->chunks[i] = (char*)malloc(CHUNK_SIZE);
        response->chunk_sizes[i] = fread(response->chunks[i], 1, CHUNK_SIZE, file);
    }

    // Check if the total bytes read match the file size
    size_t total_bytes_read = 0;
    for (size_t i = 0; i < num_chunks; i++) {
        total_bytes_read += response->chunk_sizes[i];
    }

    if (total_bytes_read < file_size) {
        size_t remaining_bytes = file_size - total_bytes_read;
        response->chunks[num_chunks - 1] = (char*)realloc(response->chunks[num_chunks - 1], remaining_bytes);
        size_t bytes_read_last_chunk = fread(response->chunks[num_chunks - 1], 1, remaining_bytes, file);
        response->chunk_sizes[num_chunks - 1] = bytes_read_last_chunk;
    }

    fclose(file);
}


HTTPDAPI response_t* new_response(Cstr path, Cstr content, Cstr type, size_t code)
{
    if(path == NULL){
        if(content == NULL || type == NULL){
            PANIC("When path is NULL, content and type must be set");
        }
    }

    if(content == NULL || type == NULL){
        if(path == NULL){
            PANIC("When content and/or type are NULL, path must be set");
        }
    }

    response_t* response = malloc(sizeof(response_t));
    
    if(path == NULL){
        response->header = header_content(content, type, code);
        make_chunks(response, 1);
        response->chunks[0] = (char*) malloc(strlen(content)+1);
        response->chunk_sizes[0] = strlen(content)+1;
        strcpy(response->chunks[0], (char*) content);
    } else {
        response->header = header_path(path, code);
        read_content(response, path);
        strcpy(response->header.Connection, "keep-alive");
    }
    
    return response;
}

HTTPDAPI response_t* error_response(size_t code)
{
    char* page_404 = ErrorPage(code);
    if(page_404 == NULL) {
        page_404 = clib_format_text("<html><body><h1>%zu %s</h1></body></html>", code, status_message(code)); // fallback html
    }

    response_t* response = new_response(NULL, page_404, "text/html", code);
    free(page_404);
    return response;
}

HTTPDAPI response_t* response(request_t request, const char* root)
{
    char* path = url_to_path(request.path, root);

    response_t* response = NULL;

    if(!clib_file_exists(path)){
        response = error_response(404);
    } else {
        response = new_response(path, NULL, NULL, 200);
    }
    free(path);

    if(response == NULL){
        response = error_response(500); // Internal Server Error
    }

    return response;
}
