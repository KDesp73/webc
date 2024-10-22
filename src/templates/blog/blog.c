#include "webc-templates/blog.h"
#include "extern/clib.h"
#include "webc-actions.h"
#include "webc-core.h"
#include "webc-templates/template.h"
#include "webc-ui.h"
#include "./utils.c"

void links(char** buffer) 
{
    
        NavLink nav_items[] = {
            (NavLink) {
                .title = "Home",
                .link = "/",
            },
            (NavLink) {
                .title = "About",
                .link = "/about",
            },
        };


    for (size_t i = 0; i < ARRAY_LEN(nav_items); ++i) {
        char* link = (char*) malloc(1); 
        memset(link, 0, 1);
        WEBC_Li(&link, NO_ATTRIBUTES, nav_items[i].title); 
        WEBC_Anchor(buffer, WEBC_UseModifier((Modifier) {.href = nav_items[i].link, .target = nav_items[i].target}), link);
        free(link);
    }
}

char* BlogLayout(Cstr title, Blog blog, BlockContents contents)
{
    char* buffer = NULL;
    WEBC_HtmlStart(&buffer, blog.lang);
    WEBC_Head(&buffer, title, 
        META_AUTHOR_TAG(blog.template.author),
        META_DESCRIPTION_TAG(blog.template.about),
        NULL
    );

    WEBC_StyleStart(&buffer, NO_ATTRIBUTES);
        if(blog.template.style_path == NULL) {
            // Get library style
        } else {
            WEBC_IntegrateFile(&buffer, blog.template.style_path);
        }
    WEBC_StyleEnd(&buffer);

    WEBC_BodyStart(&buffer, NO_ATTRIBUTES);
        WEBC_DivStart(&buffer, WEBC_UseModifier((Modifier) {.class = "sidebar"}));
            WEBC_TemplateSidebar(&buffer, blog.template, links);
        WEBC_DivEnd(&buffer);

            WEBC_MainStart(&buffer, NO_ATTRIBUTES);
                contents(&buffer);
            WEBC_MainEnd(&buffer);

    WEBC_BodyEnd(&buffer);

    WEBC_HtmlEnd(&buffer);

    return buffer;
}

void IndexContents(char** buffer)
{
    int blogs_count = 0;
    char** md_files = listMarkdownFiles(POSTS_PATH, &blogs_count);

    WEBC_UlStart(buffer, NO_ATTRIBUTES);
    for(size_t i = 0; i < blogs_count; ++i) {
        char* name = (char*) get_filename_without_extension(md_files[i]);
        char* route = clib_format_text("/posts/%s/", name);
        replaceUnderscoresAndCapitalize(name);
        WEBC_DivStart(buffer, WEBC_UseModifier((Modifier) {.class = "post"}));
            char* anchor = (char*) malloc(1);
            memset(anchor, 0, 1);
            WEBC_Anchor(&anchor, WEBC_UseModifier((Modifier) {.href = route}), name);
            WEBC_H2(buffer, NO_ATTRIBUTES, anchor);
            free(anchor);

            char* subtitle = clib_format_text("Lorem Ipsum\n");
            WEBC_Paragraph(buffer, NO_ATTRIBUTES, subtitle);
            free(subtitle);

            WEBC_Paragraph(buffer, NO_ATTRIBUTES, "Preview...");

        WEBC_DivEnd(buffer);
        free(route);
    }
    WEBC_UlEnd(buffer);
}

void AboutContents(char** buffer)
{
    WEBC_DivStart(buffer, WEBC_UseModifier((Modifier) {.class = "markdown-body", .style = "padding = 1em;"}));
        WEBC_IntegrateFile(buffer, ABOUT_PATH);
    WEBC_DivEnd(buffer);
}

char* BlogIndex(Blog blog)
{
    return BlogLayout(blog.template.title, blog, IndexContents);
}

char* BlogAbout(Blog blog)
{
    return BlogLayout("About" , blog, AboutContents);
}


char* BlogArticle(Cstr md, Blog blog)
{
    char* name = (char*) get_filename_without_extension(md);
    replaceUnderscoresAndCapitalize(name);

    char* buffer = NULL;
    WEBC_HtmlStart(&buffer, blog.lang);
    WEBC_Head(&buffer, name, 
        META_AUTHOR_TAG(blog.template.author),
        META_DESCRIPTION_TAG(blog.template.about),
        NULL
    );

    WEBC_StyleStart(&buffer, NO_ATTRIBUTES);
        if(blog.template.style_path == NULL) {
            // TODO: Get library style
        } else {
            WEBC_IntegrateFile(&buffer, blog.template.style_path);
        }

        WEBC_InfoComment(&buffer, "Markdown Style");

        WEBC_IntegrateFile(&buffer, "https://raw.githubusercontent.com/KDesp73/webc/main/style/github-markdown.css");
    WEBC_StyleEnd(&buffer);

    WEBC_BodyStart(&buffer, STYLE("margin: 0; padding: 0;"));
        WEBC_DivStart(&buffer, WEBC_UseModifier((Modifier) {.class = "markdown-body", .style = "padding = 1em;"}));
            WEBC_IntegrateFile(&buffer, md);
        WEBC_DivEnd(&buffer);
    WEBC_BodyEnd(&buffer);
    WEBC_HtmlEnd(&buffer);
    
    return buffer;
}

WEBCAPI Tree WEBC_TemplateBlog(Cstr root, Blog blog)
{
    int blogs_count = 0;
    char** md_files = listMarkdownFiles(POSTS_PATH, &blogs_count);

    if(blogs_count <= 0)
        ERRO("No articles found");

    size_t capacity = 2 + blogs_count; // Index + About + blogs_count
    Tree tree = WEBC_AllocTree(root, capacity);

    WEBC_AddRoute(&tree, WEBC_MakeRoute("/", BlogIndex(blog)));
    WEBC_AddRoute(&tree, WEBC_MakeRoute("/about", BlogAbout(blog)));

    
    for(size_t i = 0; i < blogs_count; ++i){
        char* route = clib_format_text("/posts/%s/", get_filename_without_extension(md_files[i]));
        WEBC_AddRoute(&tree, WEBC_MakeRoute(route, BlogArticle(md_files[i], blog)));
        free(route);
    }

    return tree;
}
