#include "webc-core.h"
#define DAISY_UI
#include "webc-ui.h"



WEBCAPI void WEBC_DaisyBadge(char** buffer, BadgeType type, Cstr color, Cstr text)
{
    if(!is_valid_daisy_color(color)){
        ERRO("Invalid DaisyUI color at DaisyBadge: %s", SECURE_STR(color));
        return;
    }

    char* badge_class = clib_format_text("badge");

    append_class_attr(&badge_class, color);

    if(type == BADGE_OUTLINE){
        WEBC_Append(&badge_class, " badge-outline");
    }

    WEBC_DivStart(buffer, CLASS(badge_class));
        WEBC_PlainText(buffer, text);
    WEBC_DivEnd(buffer);

    free(badge_class);
}
