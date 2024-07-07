#include "webc-core.h"
#include "webc-ui.h"


WEBCAPI void WEBC_DaisyCode(char** buffer, Cstr color, CodeLine code[], size_t count)
{
    if(!is_valid_daisy_color(color)){
        ERRO("Invalid DaisyUI color: %s at DaisyCode", SECURE_STR(color));
        return;
    }

    char* class = clib_format_text("mockup-code");
    if(color != NULL){
        char* color_bg = clib_format_text(" bg-%s", color);
        char* color_fg = clib_format_text(" text-%s-content", color);

        WEBC_Append(&class, color_bg);
        WEBC_Append(&class, color_fg);

        free(color_bg);
        free(color_fg);
    }

    WEBC_DivStart(buffer, CLASS(class));
        for(size_t i = 0; i < count; ++i){
            char* code_str = WEBC_BufferInit();
            WEBC_Code(&code_str, NO_ATTRIBUTES, code[i].code);
            WEBC_Pre(buffer, WEBC_UseModifier((Modifier) {.data_prefix = code[i].prefix}), code_str);
            free(code_str);
        }
    WEBC_DivEnd(buffer);

    free(class);
}
