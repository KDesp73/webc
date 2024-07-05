#include "webc-core.h"
#define DAISY_UI
#include "webc-ui.h"

WEBCAPI void WEBC_DaisyCollapse(char** buffer, Cstr title, Cstr contents)
{
    Modifier collapse_modifier = {
        .tabindex = "0",
        .class = "collapse collapse-arrow border-base-300 bg-base-200 border"
    };

    WEBC_DivStart(buffer, WEBC_UseModifier(collapse_modifier));
        WEBC_DivStart(buffer, CLASS("collapse-title text-xl font-medium"));
            WEBC_PlainText(buffer, title);
        WEBC_DivEnd(buffer);
        WEBC_DivStart(buffer, CLASS("collapse-content"));
            WEBC_Paragraph(buffer, NO_ATTRIBUTES, contents);
        WEBC_DivEnd(buffer);
    WEBC_DivEnd(buffer);
}
