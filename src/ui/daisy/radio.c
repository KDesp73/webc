#include "webc-core.h"
#include "webc-ui.h"


WEBCAPI void WEBC_DaisyRadio(char** buffer, Cstr group_name, Cstr color, int checked)
{
    assert(group_name != NULL);

    char* class = clib_format_text("radio");
    
    append_class_attr(&class, color);

    Modifier mod = {
        .type = "radio",
        .name = group_name,
        .class = class,
        .checked = (checked) ? "checked" : NULL
    };


    WEBC_Input(buffer, WEBC_UseModifier(mod));
    free(class);
}
