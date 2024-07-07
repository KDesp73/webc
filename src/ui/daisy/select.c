#include "webc-core.h"
#include "webc-ui.h"


WEBCAPI void WEBC_DaisySelect(char** buffer, Cstr title, char* options[], size_t count, Cstr color)
{
    if(!is_valid_daisy_color(color)){
        ERRO("Invalid DaisyUI color: %s at DaisySelect", SECURE_STR(color));
        return;
    }

    char* class = clib_format_text("select w-full max-w-xs");

    if(color != NULL) {
        char* c = clib_format_text(" select-%s", color); 
        WEBC_Append(&class, c);
        free(c);
    }

    WEBC_SelectStart(buffer, CLASS(class));
        WEBC_Option(buffer, WEBC_UseModifier((Modifier) {.disabled = 1, .selected = 1}), title);
        for(size_t i = 0; i < count; ++i){
            WEBC_Option(buffer, NO_ATTRIBUTES, options[i]);
        }
    WEBC_SelectEnd(buffer);

    free(class);

}
