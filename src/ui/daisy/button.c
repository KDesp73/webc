#include "webc-core.h"
#include "webc-ui.h"


WEBCAPI void WEBC_DaisyButton(char** buffer, ButtonType type, Button button)
{
    char* class = NULL;
    switch (type) {
        case BUTTON_DEFAULT:
        case BUTTON_ICON:
        case BUTTON_LOADING_TEXT:
            class = clib_format_text("btn");
            break;
        case BUTTON_OUTLINE:
            class = clib_format_text("btn btn-outline");
            break;
        case BUTTON_RESPONSIVE:
            class = clib_format_text("btn btn-xs sm:btn-sm md:btn-md lg:btn-lg");
            break;
        case BUTTON_WIDE:
            class = clib_format_text("btn btn-wide");
            break;
        case BUTTON_GLASS:
            class = clib_format_text("btn glass");
            break;
        case BUTTON_SQUARE:
        case BUTTON_LOADING:
            class = clib_format_text("btn btn-square");
            break;
        case BUTTON_CIRCLE:
            class = clib_format_text("btn btn-circle");
            break;
        default:
            return;
    }

    if(button.color != NULL){
        char* c = clib_format_text(" btn-%s", button.color);
        WEBC_Append(&class, c);
        free(c);
    }

    Modifier mod = {
        .class = class,
        .onclick = button.onclick
    };
    WEBC_ButtonStart(buffer, WEBC_UseModifier(mod));
        if(button.svg != NULL)
            WEBC_PlainText(buffer, button.svg);
        
        if(type == BUTTON_LOADING || type == BUTTON_LOADING_TEXT)
            WEBC_Span(buffer, CLASS("loading loading-spinner"), "");

        if(type != BUTTON_LOADING)
            WEBC_PlainText(buffer, button.text);
    WEBC_ButtonEnd(buffer);

}

