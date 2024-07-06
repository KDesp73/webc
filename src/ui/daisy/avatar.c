#include "webc-core.h"
#define DAISY_UI
#include "webc-ui.h"


WEBCAPI void WEBC_DaisyAvatar(char** buffer, AvatarType type, Cstr img_src, Cstr placeholder, size_t width)
{
    char* rounded = NULL;
    if(type == AVATAR_DEFAULT){
        rounded = "rounded";
    } else {
        rounded = "rounded-full";
    }
    char* avatar_class = clib_format_text("w-%zu %s", width, rounded);

    if(type == AVATAR_RING){
        WEBC_Append(&avatar_class, " ring-primary ring-offset-base-100 ring ring-offset-2");
    }

    if(img_src == NULL){
        WEBC_Append(&avatar_class, " bg-neutral text-neutral-content");
    }

    char* avatar_container_class = clib_format_text("avatar");
    if(type == AVATAR_PRESENCE_ONLINE){
        WEBC_Append(&avatar_container_class, " online");
    } else if(type == AVATAR_PRESENCE_OFFLINE) {
        WEBC_Append(&avatar_container_class, " offline");
    } 

    if(img_src == NULL){
        WEBC_Append(&avatar_container_class, " placeholder");
    }

    char* span_class = clib_format_text("test-");
    if(width > 20) {
        WEBC_Append(&span_class, "3xl");
    } else if(width <= 20 && width > 14){
        WEBC_Append(&span_class, "xl");
    } else if(width <= 14 && width > 10) {
        span_class = ""; // no class
    } else if(width <= 10) {
        WEBC_Append(&span_class, "xs");
    }

    WEBC_DivStart(buffer, CLASS(avatar_container_class));
        WEBC_DivStart(buffer, CLASS(avatar_class));
            if(img_src == NULL){
                WEBC_Span(buffer, CLASS(span_class), (placeholder != NULL) ? placeholder : "NULL");
            } else {
                WEBC_Img(buffer, WEBC_UseModifier((Modifier) {.src = img_src}));
            }
        WEBC_DivEnd(buffer);
    WEBC_DivEnd(buffer);

    free(avatar_class);
    free(span_class);
    free(avatar_container_class);
}
