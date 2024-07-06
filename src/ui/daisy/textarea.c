#include "webc-core.h"
#include "webc-ui.h"


WEBCAPI void WEBC_DaisyTextarea(char** buffer, Cstr placeholder, Cstr color, Cstr size)
{
    char* class = clib_format_text("textarea");

    if(color != NULL) {
        char* c = clib_format_text(" %s-%s", class, color); 
        WEBC_Append(&class, c);
        free(c);
    }

    if(size != NULL) {
        char* s = clib_format_text(" %s-%s", class, size); 
        WEBC_Append(&class, s);
        free(s);
    }

    WEBC_Textarea(buffer, WEBC_UseModifier((Modifier) {.class = class, .placeholder = placeholder}), NULL);

    free(class);
}
