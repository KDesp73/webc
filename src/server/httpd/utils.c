#include "extern/httpd.h"
#include <string.h>

HTTPDAPI int has_file_extension(const char *path) 
{
    const char *extension = strrchr(path, '.');
    
    if (extension != NULL) {
        if (*(extension + 1) != '\0') {
            return true;
        }
    }
    
    return false;
} 

HTTPDAPI int is_index_html_needed(const char *url) 
{
    size_t url_len = strlen(url);
    if (url_len == 0 || url[url_len - 1] == '/' || !has_file_extension(url)) {
        return true;
    } else {
        return false;
    }
}

HTTPDAPI char* url_to_path(const char* url, const char* root)
{ 
    char* path = NULL;
    size_t url_len = strlen(url);
    if(is_index_html_needed(url)) {
        if(url[url_len - 1] == '/'){
            path = clib_format_text("%s%sindex.html", root, url);
        } else {
            path = clib_format_text("%s/%s/index.html", root, url);
        }
    } else {
        path = clib_format_text("%s%s", root, url);
    }

    return path;
}
