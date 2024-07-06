#include "webc-core.h"
#include "webc-ui.h"


WEBCAPI void WEBC_DaisyTextInput(char** buffer, int password, Cstr placeholder, Cstr icon)
{
    Modifier mod = {
        .type = password ? "password" : "text", 
        .class = "grow", 
        .placeholder = placeholder
    };

    WEBC_LabelStart(buffer, CLASS("input input-bordered flex items-center gap-2"));
        if(icon != NULL) WEBC_PlainText(buffer, icon);
        WEBC_Input(buffer, WEBC_UseModifier(mod));
    WEBC_LabelEnd(buffer);
}
