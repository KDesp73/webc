#include "webc-core.h"
#include "webc-ui.h"

WEBCAPI void WEBC_DaisyLoading(char** buffer, LoadingType type, Cstr size)
{
    if(!is_valid_daisy_size(size)){
        ERRO("Invalid DaisyUI size: %s", SECURE_STR(size));
        return;
    }

    char* loading = NULL;
    switch (type) {
        case LOADING_SPINNER:
            loading = "spinner";
            break;
        case LOADING_DOTS:
            loading = "dots";
            break;
        case LOADING_RING:
            loading = "ring";
            break;
        case LOADING_BALL:
            loading = "ball";
            break;
        case LOADING_BAR:
            loading = "bar";
            break;
        case LOADING_INFINITY:
            loading = "infinity";
            break;
    }

    char* class = clib_format_text("loading loading-%s loading-%s", loading, size);
    
    WEBC_Span(buffer, CLASS(class), "");

    free(class);
}
