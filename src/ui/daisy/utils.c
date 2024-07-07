#include "webc-ui.h"

void append_class_attr(char** class, Cstr attr)
{
    if(attr != NULL) {
        char* c = clib_format_text(" %s-%s", *class, attr); 
        WEBC_Append(class, c);
        free(c);
    }
}

int is_valid_daisy_color(Cstr color)
{
    return color != NULL && (
        strcmp(color, DAISY_COLOR_PRIMARY) == 0 ||
        strcmp(color, DAISY_COLOR_PRIMARY_CONTENT) == 0 ||
        strcmp(color, DAISY_COLOR_SECONDARY) == 0 ||
        strcmp(color, DAISY_COLOR_SECONDARY_CONTENT) == 0 ||
        strcmp(color, DAISY_COLOR_ACCENT) == 0 ||
        strcmp(color, DAISY_COLOR_ACCENT_CONTENT) == 0 ||
        strcmp(color, DAISY_COLOR_NEUTRAL) == 0 ||
        strcmp(color, DAISY_COLOR_NEUTRAL_CONTENT) == 0 ||
        strcmp(color, DAISY_COLOR_BASE_100) == 0 ||
        strcmp(color, DAISY_COLOR_BASE_200) == 0 ||
        strcmp(color, DAISY_COLOR_BASE_300) == 0 ||
        strcmp(color, DAISY_COLOR_INFO) == 0 ||
        strcmp(color, DAISY_COLOR_INFO_CONTENT) == 0 ||
        strcmp(color, DAISY_COLOR_SUCCESS) == 0 ||
        strcmp(color, DAISY_COLOR_SUCCESS_CONTENT) == 0 ||
        strcmp(color, DAISY_COLOR_WARNING) == 0 ||
        strcmp(color, DAISY_COLOR_WARNING_CONTENT) == 0 ||
        strcmp(color, DAISY_COLOR_ERROR) == 0 ||
        strcmp(color, DAISY_COLOR_ERROR_CONTENT) == 0
    );
}

int is_valid_daisy_size(Cstr size)
{
    return size != NULL && (
        strcmp(size, DAISY_SIZE_XS) == 0 ||
        strcmp(size, DAISY_SIZE_SM) == 0 ||
        strcmp(size, DAISY_SIZE_MD) == 0 ||
        strcmp(size, DAISY_SIZE_LG) == 0
    );

}
