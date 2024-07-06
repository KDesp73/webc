#include "webc-core.h"
#include "webc-ui.h"

void render_items(char** buffer, NavLink items[], size_t count)
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


WEBCAPI void WEBC_DaisyDropdown(char** buffer, Cstr title, NavLink items[], size_t count)
{
    WEBC_DivStart(buffer, CLASS("dropdown"));
        WEBC_DivStart(buffer, WEBC_UseModifier((Modifier) {.tabindex = "0", .role = "button", .class = "btn m-1"}));
            WEBC_PlainText(buffer, title);
        WEBC_DivEnd(buffer);
        WEBC_UlStart(buffer, WEBC_UseModifier((Modifier) {.tabindex = "0", .class = "dropdown-content menu bg-base-100 rounded-box z-[1] w-52 p-2 shadow"}));
            render_items(buffer, items, count);
        WEBC_UlEnd(buffer);
    WEBC_DivEnd(buffer);
}
