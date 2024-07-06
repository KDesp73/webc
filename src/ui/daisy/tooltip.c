#include "webc-core.h"
#include "webc-ui.h"

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
