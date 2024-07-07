#include "webc-core.h"
#include "webc-ui.h"


WEBCAPI void WEBC_DaisyProgress(char** buffer, size_t width, Cstr color, size_t value, size_t max)
{
    if(!is_valid_daisy_color(color)){
        ERRO("Invalid DaisyUI color: %s at DaisyProgress", SECURE_STR(color));
        return;
    }

    char* class = clib_format_text("progress w-%zu", width);

    if(color != NULL){
        char* c = clib_format_text(" progress-%s", color);
        WEBC_Append(&class, c); // Memory leak???
        free(c);
    }

    if(max == 0){
        WEBC_Progress(buffer, CLASS(class), "");
        return;
    }

    Modifier mod = {
        .class = class,
        .value = clib_format_text("%zu", value),
        .max = clib_format_text("%zu", max)
    };
    WEBC_Progress(buffer, WEBC_UseModifier(mod), "");

    free(class);
    free((char*) mod.value);
    free((char*) mod.max);
} 
