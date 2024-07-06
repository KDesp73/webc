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

WEBCAPI void WEBC_DaisyToast(char** buffer, Cstr text, Cstr color)
{
    char* class = clib_format_text("alert");
    append_class_attr(&class, color);

    WEBC_DivStart(buffer, CLASS("toast"));
        WEBC_DivStart(buffer, CLASS(class));
            WEBC_Span(buffer, NO_ATTRIBUTES, text);
        WEBC_DivEnd(buffer);
    WEBC_DivEnd(buffer);

    free(class);
}
