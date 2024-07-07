#include "webc-core.h"
#include "webc-ui.h"


WEBCAPI void WEBC_DaisyModal(char** buffer, Cstr id, Cstr title, Cstr content)
{
    Modifier dialog_modifier = {
        .class = "modal",
        .id = (id != NULL) ? id : "my_modal"
    };

    WEBC_DialogStart(buffer, WEBC_UseModifier(dialog_modifier));
        WEBC_DivStart(buffer, CLASS("modal-box"));
            WEBC_H3(buffer, CLASS("text-lg font-bold"), title);
            WEBC_Paragraph(buffer, CLASS("py-4"), content);
            WEBC_DivStart(buffer, CLASS("modal-action"));
                WEBC_FormStart(buffer, CLASS("dialog"));
                    WEBC_Button(buffer, CLASS("btn"), "Close");
                WEBC_FormEnd(buffer);
            WEBC_DivEnd(buffer);
        WEBC_DivEnd(buffer);
    WEBC_DialogEnd(buffer);
}
