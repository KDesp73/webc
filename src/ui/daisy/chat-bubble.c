#include "webc-core.h"
#include <time.h>
#define DAISY_UI
#include "webc-ui.h"

int is_image_ready(ChatBubbleType type, ChatBubble bubble)
{
    return  (type == CHAT_BUBBLE_IMAGE ||
             type == CHAT_BUBBLE_FULL) &&
            (bubble.img_src != NULL);
}

int is_header_footer_ready(ChatBubbleType type, ChatBubble bubble)
{
    return  (type == CHAT_BUBBLE_HEADER_FOOTER ||
             type == CHAT_BUBBLE_FULL) &&
            (bubble.name != NULL &&
             bubble.footer != NULL);
}

WEBCAPI void WEBC_DaisyChatBubble(char** buffer, ChatBubbleType type, ChatBubble bubble, int left)
{
    char* bubble_class = NULL;
    bubble_class = clib_format_text("chat chat-%s", (left) ? "start" : "end");

    if(bubble.color != NULL){
        char* color = clib_format_text(" chat-bubble-%s", bubble.color);
        WEBC_Append(&bubble_class, color);
        free(color);
    }

    WEBC_DivStart(buffer, CLASS(bubble_class));
        if(is_image_ready(type, bubble)){
            WEBC_DivStart(buffer, CLASS("chat-image avatar"));
                WEBC_DivStart(buffer, CLASS("w-10 rounded-full"));
                    WEBC_Img(buffer, SRC(bubble.img_src));
                WEBC_DivEnd(buffer);
            WEBC_DivEnd(buffer);
        }

        if(is_header_footer_ready(type, bubble)){
            WEBC_DivStart(buffer, CLASS("chat-header"));
                WEBC_PlainText(buffer, bubble.name);
                if(bubble.time != NULL){
                    WEBC_Time(buffer, CLASS("text-xs opacity-50"), bubble.time);
                }
            WEBC_DivEnd(buffer);
        }
        WEBC_DivStart(buffer, CLASS("chat-bubble"));
            WEBC_PlainText(buffer, bubble.text); // TODO: Replace '\n' with '<br>'
        WEBC_DivEnd(buffer);
        if(is_header_footer_ready(type, bubble)){
            WEBC_DivStart(buffer, CLASS("chat-footer opacity-50"));
                WEBC_PlainText(buffer, bubble.footer);
            WEBC_DivEnd(buffer);
        }
    WEBC_DivEnd(buffer);
    free(bubble_class);
}
