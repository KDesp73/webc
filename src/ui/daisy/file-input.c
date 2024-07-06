#include "webc-core.h"
#include "webc-ui.h"


WEBCAPI void WEBC_DaisyFileInput(char** buffer, Cstr color)
{
    char* class = clib_format_text("file-input file-input-bordered w-full max-w-xs");

    if(color != NULL){
        char* c = clib_format_text(" file-input-%s", color);
        WEBC_Append(&class, c);
        free(c);
    }

    WEBC_Input(buffer, WEBC_UseModifier((Modifier) {.type = "file", .class = class}));

    free(class);
}
