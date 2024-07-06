#include "webc-core.h"
#include "webc-ui.h"


WEBCAPI void WEBC_DaisyRadio(char** buffer, Cstr group_name, Cstr color, int checked)
{
    assert(group_name != NULL);

    char* class = clib_format_text("radio");
    
    if(color != NULL){
        char* c = clib_format_text(" radio-%s", color);
        WEBC_Append(&class, c);
        free(c);
    }

    Modifier mod = {
        .type = "radio",
        .name = group_name,
        .class = class
    };

    if(checked){
        mod.checked = "checked";
    }

    WEBC_Input(buffer, WEBC_UseModifier(mod));
    free(class);
}
