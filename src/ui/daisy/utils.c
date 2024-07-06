#include "webc-ui.h"

void append_class_attr(char** class, Cstr attr)
{
    if(attr != NULL) {
        char* c = clib_format_text(" %s-%s", *class, attr); 
        WEBC_Append(class, c);
        free(c);
    }
}
