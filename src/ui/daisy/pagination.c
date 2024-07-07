#include "webc-core.h"
#include "webc-ui.h"

WEBCAPI void WEBC_DaisyPagination(char** buffer, size_t from, size_t to, size_t checked)
{
    assert(from < to);
    assert(from <= checked && checked < to);

    WEBC_DivStart(buffer, CLASS("join"));
        for(size_t i = from; i < to; ++i){
            char* index = clib_format_text("%zu", i);
            Modifier mod = {
                .class = "join-item btn btn-square",
                .type = "radio",
                .name = "options",
                .aria_label = index,
                .checked = (i == checked) ? "checked" : NULL
            };

            WEBC_Input(buffer, WEBC_UseModifier(mod));
            free(index);
        }
    WEBC_DivEnd(buffer);
}
