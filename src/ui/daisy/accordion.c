#include "webc-core.h"
#define DAISY_UI
#include "webc-ui.h"


WEBCAPI void WEBC_DaisyAccordion(char** buffer, Accordion accordion, int open)
{
    Modifier input_modifier = {
        .type = "radio",
        .name = "my-accordion-2"
    };

    if(open){
        input_modifier.checked = "checked";
    }

    WEBC_DivStart(buffer, CLASS("collapse collapse-arrow bg-base-200"));
        WEBC_Input(buffer, WEBC_UseModifier(input_modifier));
        WEBC_DivStart(buffer, CLASS("collapse-title text-xl font-medium"));
            WEBC_PlainText(buffer, accordion.title);
        WEBC_DivEnd(buffer);
        WEBC_DivStart(buffer, CLASS("collapse-content"));
            WEBC_Paragraph(buffer, NO_ATTRIBUTES, accordion.content);
        WEBC_DivEnd(buffer);
    WEBC_DivEnd(buffer);
}


