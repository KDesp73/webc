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

WEBCAPI void WEBC_DaisyTooltip(char** buffer, Cstr tooltip, Cstr location, int open, Cstr color, BlockContents toplaceon)
{
    assert(toplaceon != NULL);
    char* class = clib_format_text("tooltip");

    append_class_attr(&class, color);
    append_class_attr(&class, location);

    WEBC_DivStart(buffer, WEBC_UseModifier((Modifier) {.class = class, .data_tip = tooltip}));
        toplaceon(buffer);
    WEBC_DivEnd(buffer);
}
