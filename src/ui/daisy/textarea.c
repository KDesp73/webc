#include "webc-core.h"
#include "webc-ui.h"


WEBCAPI void WEBC_DaisyTextarea(char** buffer, Cstr placeholder, Cstr color, Cstr size)
{
    if(!is_valid_daisy_color(color)){
        ERRO("Invalid DaisyUI color: %s at DaisyTextarea", SECURE_STR(color));
        return;
    }
    if(!is_valid_daisy_size(size)){
        ERRO("Invalid DaisyUI size: %s at DaisyTextarea", SECURE_STR(size));
        return;
    }

    char* class = clib_format_text("textarea");

    if(color != NULL) {
        char* c = clib_format_text(" %s-%s", class, color); 
        WEBC_Append(&class, c);
        free(c);
    }

    if(size != NULL) {
        char* s = clib_format_text(" %s-%s", class, size); 
        WEBC_Append(&class, s);
        free(s);
    }

    WEBC_Textarea(buffer, WEBC_UseModifier((Modifier) {.class = class, .placeholder = placeholder}), NULL);

    free(class);
}
