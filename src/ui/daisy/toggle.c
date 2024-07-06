#include "webc-core.h"
#include "webc-ui.h"


void append_class_attr(char** class, Cstr attr)
{
    if(attr != NULL) {
        char* c = clib_format_text(" %s-%s", *class, attr); 
        WEBC_Append(class, c);
        free(c);
    }
}


WEBCAPI void WEBC_DaisyToggle(char** buffer, int checked, Cstr color, Cstr size)
{
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
