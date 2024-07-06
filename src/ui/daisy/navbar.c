#include "webc-core.h"
#include "webc-ui.h"

const char* burger =
"<svg\n"
"xmlns=\"http://www.w3.org/2000/svg\"\n"
"fill=\"none\"\n"
"viewBox=\"0 0 24 24\"\n"
"class=\"inline-block h-6 w-6 stroke-current\">\n"
"<path\n"
"stroke-linecap=\"round\"\n"
"stroke-linejoin=\"round\"\n"
"stroke-width=\"2\"\n"
"d=\"M4 6h16M4 12h16M4 18h16\"></path>\n"
"</svg>\n";

void items(char** buffer, NavLink items[], size_t count)
{
    
    for(size_t i = 0; i < count; ++i){
        NavLink item = items[i];
        char* item_str = clib_format_text("<li><a href=\"%s\" target=\"%s\">%s</a></li>", 
                (item.link == NULL) ? "" : item.link,
                (item.target == NULL) ? "" : item.target,
                (item.title == NULL) ? "" : item.title
                );
        WEBC_PlainText(buffer, item_str);
        free(item_str);
    }
}

WEBCAPI void WEBC_DaisyNavbarSidebar(
        char** buffer, 
        Cstr title, 
        NavLink navbar_items[],
        size_t navbar_items_count,
        NavLink sidebar_items[],
        size_t sidebar_items_count,
        BlockContents page_content)
{
    Modifier label_modifier = {
        .for_ = "my-drawer-3", 
        .aria_label = "open drawer", 
        .class = "btn btn-square btn-ghost"
    };

    WEBC_DivStart(buffer, CLASS("drawer")); 
        WEBC_Input(buffer, WEBC_UseModifier((Modifier) {.id="my-drawer-3", .type = "checkbox", .class = "drawer-toggle"}));
        WEBC_DivStart(buffer, CLASS("drawer-content flex flex-col"));
            WEBC_DivStart(buffer, CLASS("navbar bg-base-300 w-full"));
                WEBC_DivStart(buffer, CLASS("flex-none lg:hidden"));
                    WEBC_LabelStart(buffer, WEBC_UseModifier(label_modifier));
                        WEBC_PlainText(buffer, burger); // Icon
                    WEBC_LabelEnd(buffer);
                WEBC_DivEnd(buffer);
                WEBC_DivStart(buffer, CLASS("mx-2 flex-1 px-2"));WEBC_PlainText(buffer, title);WEBC_DivEnd(buffer); // Title
                WEBC_DivStart(buffer, CLASS("hidden flex-none lg:block"));
                    WEBC_UlStart(buffer, CLASS("menu menu-horizontal"));
                        items(buffer, navbar_items, navbar_items_count);
                    WEBC_UlEnd(buffer);
                WEBC_DivEnd(buffer);
            WEBC_DivEnd(buffer);
            page_content(buffer);
        WEBC_DivEnd(buffer);
        WEBC_DivStart(buffer, CLASS("drawer-side"));
            WEBC_Label(buffer, WEBC_UseModifier(label_modifier), "");
            WEBC_UlStart(buffer, CLASS("menu bg-base-200 min-h-full w-80 p-4"));
                if(sidebar_items_count == 0 || sidebar_items == NULL){
                    items(buffer, navbar_items, navbar_items_count);
                } else {
                    items(buffer, sidebar_items, sidebar_items_count);
                }
            WEBC_UlEnd(buffer);
        WEBC_DivEnd(buffer);
    WEBC_DivEnd(buffer);
}
