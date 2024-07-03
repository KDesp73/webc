#include "webc-md.h"
#include "extern/cmark/cmark.h"

static char* convert(cmark_node *document, 
        int options, int width) {
    char *result;

    result = cmark_render_html(document, options);

    return result;
}


WEBCAPI Cstr WEBC_MarkdownToHtml(Cstr path)
{
    char buffer[4096];
    cmark_parser *parser;
    size_t bytes;
    cmark_node *document;
    int width = 0;
    int options = CMARK_OPT_DEFAULT;
    options |= CMARK_OPT_UNSAFE;
    options |= CMARK_OPT_VALIDATE_UTF8;

    parser = cmark_parser_new(options);
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file %s: %s\n", path,
                strerror(errno));
        exit(1);
    }

    while ((bytes = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
        cmark_parser_feed(parser, buffer, bytes);
        if (bytes < sizeof(buffer)) {
            break;
        }
    }

    fclose(fp);

    document = cmark_parser_finish(parser);
    cmark_parser_free(parser);

    char* result = convert(document, options, width);
    cmark_node_free(document);

    return result;
}

WEBCAPI Cstr WEBC_MarkdownToHtmlText(Cstr md)
{
    cmark_parser *parser;
    cmark_node *document;
    int width = 0;
    int options = CMARK_OPT_DEFAULT;
    options |= CMARK_OPT_UNSAFE;
    options |= CMARK_OPT_VALIDATE_UTF8;
    
    parser = cmark_parser_new(options);
    cmark_parser_feed(parser, md, strlen(md));
    document = cmark_parser_finish(parser);
    cmark_parser_free(parser);

    char* result = convert(document, options, width);
    cmark_node_free(document);

    return result;
}
