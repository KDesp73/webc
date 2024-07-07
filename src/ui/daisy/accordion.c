#include "webc-core.h"
#define DAISY_UI
#include "webc-ui.h"


WEBCAPI void WEBC_DaisyAccordion(char** buffer, Accordion accordion, int open)
{
    if(accordion.title == NULL){
        ERRO("accordion.title is NULL in DaisyAccordion");
        return;
    }

    if(accordion.content == NULL){
        ERRO("accordion.content is NULL in DaisyAccordion");
        return;
    }

    Modifier input_modifier = {
        .type = "radio",
        .name = "my-accordion-2",
        .checked = (open) ? "checked" : NULL
    };

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


