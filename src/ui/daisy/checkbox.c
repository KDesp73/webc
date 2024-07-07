#include "webc-core.h"
#include "webc-ui.h"


WEBCAPI void WEBC_DaisyCheckbox(char** buffer, Cstr label, Cstr color)
{
    if(!is_valid_daisy_color(color)){
        ERRO("Invalid DaisyUI color: %s at DaisyCheckbox", SECURE_STR(color));
        return;
    }

    char* class = clib_format_text("checkbox");
    if(color != NULL){
        char* c = clib_format_text(" checkbox-%s", color);
        WEBC_Append(&class, c);
        free(c);
    }
    Modifier mod = {
        .type = "checkbox", 
        .class = class
    };

    WEBC_DivStart(buffer, CLASS("form-control"));
        WEBC_LabelStart(buffer, CLASS("label cursor-pointer"));
        if(label != NULL)
            WEBC_Span(buffer, CLASS("label-text"), label);
        WEBC_Input(buffer, WEBC_UseModifier(mod));
        WEBC_LabelEnd(buffer);
    WEBC_DivEnd(buffer);

    free(class);
}
