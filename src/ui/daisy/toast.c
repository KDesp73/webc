#include "webc-core.h"
#include "webc-ui.h"


WEBCAPI void WEBC_DaisyToast(char** buffer, Cstr text, Cstr color)
{
    if(!is_valid_daisy_color(color)){
        ERRO("Invalid DaisyUI color: %s at DaisyToast", SECURE_STR(color));
        return;
    }

    char* class = clib_format_text("alert");
    append_class_attr(&class, color);

    WEBC_DivStart(buffer, CLASS("toast"));
        WEBC_DivStart(buffer, CLASS(class));
            WEBC_Span(buffer, NO_ATTRIBUTES, text);
        WEBC_DivEnd(buffer);
    WEBC_DivEnd(buffer);

    free(class);
}
