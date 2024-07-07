#include "webc-core.h"
#include "webc-ui.h"


WEBCAPI void WEBC_DaisyRange(char** buffer, size_t min, size_t max, size_t value, size_t step, Cstr size, Cstr color)
{
    if(!is_valid_daisy_color(color)){
        ERRO("Invalid DaisyUI color: %s at DaisyRange", SECURE_STR(color));
        return;
    }
    if(!is_valid_daisy_size(size)){
        ERRO("Invalid DaisyUI size: %s at DaisyRange", SECURE_STR(size));
        return;
    }
    
    char* class = clib_format_text("range");

    if(color != NULL) {
        char* c = clib_format_text(" range-%s", color); 
        WEBC_Append(&class, c);
        free(c);
    }

    if(size != NULL) {
        char* s = clib_format_text(" range-%s", size); 
        WEBC_Append(&class, s);
        free(s);
    }

    Modifier mod = {
        .type = "range",
        .min = clib_format_text("%zu", min),
        .max = clib_format_text("%zu", max),
        .value = clib_format_text("%zu", value),
        .step = clib_format_text("%zu", step),
        .class = class
    };

    WEBC_Input(buffer, WEBC_UseModifier(mod));
    if(step != 0){
        WEBC_DivStart(buffer, CLASS("flex w-full justify-between px-2 text-xs"));
        for(size_t i = 0; i <= max / step; ++i){
            WEBC_Span(buffer, NO_ATTRIBUTES, "|");
        }
        WEBC_DivEnd(buffer);
    }

    free(class);
    free((char*) mod.min);
    free((char*) mod.max);
    free((char*) mod.value);
    free((char*) mod.step);
}
