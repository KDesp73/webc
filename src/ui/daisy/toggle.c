#include "webc-core.h"
#include "webc-ui.h"


WEBCAPI void WEBC_DaisyToggle(char** buffer, int checked, Cstr color, Cstr size)
{
    if(!is_valid_daisy_color(color)){
        ERRO("Invalid DaisyUI color: %s at DaisyToggle", SECURE_STR(color));
        return;
    }
    if(!is_valid_daisy_size(size)){
        ERRO("Invalid DaisyUI size: %s at DaisyToggle", SECURE_STR(size));
        return;
    }

    char* class = clib_format_text("toggle");
    append_class_attr(&class, color);
    append_class_attr(&class, size);

    Modifier mod = {
        .type = "checkbox",
        .class = class,
        .checked = (checked) ? "checked" : NULL
    };

    WEBC_Input(buffer, WEBC_UseModifier(mod));
}
