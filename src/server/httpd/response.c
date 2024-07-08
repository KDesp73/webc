#include "extern/httpd.h"
#include "webc-core.h"
#include <string.h>


void append_key(char** buffer, const char* key, const char* value)
{
    if(value == NULL) return;
    if(key == NULL) return;
    if(*buffer == NULL) return;

    char* key_value = clib_format_text("%s: %s\r", key, value);
    clib_str_append_ln(buffer, key_value);
    free(key_value);
}

HTTPDAPI const char* response_str(response_t response)
{
    char* response_str = clib_format_text("HTTP/1.1 %zu %s\r\n", response.header.status_code, status_message(response.header.status_code));
    
    append_key(&response_str, "Server", response.header.Server);
    append_key(&response_str, "Content-Type", response.header.Content_Type);
    char* length = clib_format_text("%zu", response.header.Content_Length);
    append_key(&response_str, "Content-Length", length);
    free(length);
    append_key(&response_str, "Connection", response.header.Connection);
    append_key(&response_str, "Date", response.header.Date);

    clib_str_append(&response_str, "\r\n");
    clib_str_append(&response_str, "\r\n");
    clib_str_append_ln(&response_str, response.content);

    return response_str;
}

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
    free(response->content);
}

char* read_image(const char *file_path) 
{
    FILE *file = fopen(file_path, "rb");

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *image_data = (char *)malloc(file_size);
    fread(image_data, 1, file_size, file);
    fclose(file);

    return image_data;
}

int is_image_file(const char *file_path) 
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

HTTPDAPI response_t response(request_t request, const char* root)
{
    char* path = url_to_path(request.path, root);

    response_t response;

    char* page_404 = ErrorPage(404);
    if(!clib_file_exists(path)){
        response = (response_t) {
            .header = header_content(page_404, "text/html", 404),
            .content = page_404
        };
    } else {
        if(is_image_file(path)){
            INFO("%s is image", path);
            response = (response_t){
                .header = header_path(path, 200),
                .content = read_image(path)
            };
        } else {
            response = (response_t){
                .header = header_path(path, 200),
                .content = clib_read_file(path, "r")
            };
        }
    }

    free(path);
    return response;
}
