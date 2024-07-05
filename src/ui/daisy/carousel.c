#include "webc-core.h"
#define DAISY_UI
#include "webc-ui.h"

WEBCAPI void WEBC_DaisyCarousel(char** buffer, CarouselType type, size_t width, CarouselItem items[], size_t item_count)
{
    char* carousel_class = clib_format_text("carousel");

    if(width != 0){
        char* w = clib_format_text(" w-%zu", width);
        WEBC_Append(&carousel_class, w);
        free(w);
    }

    switch (type) {
        case CAROUSEL_DEFAULT:
            break;
        case CAROUSEL_FULL_WIDTH:
        case CAROUSEL_HALF_WIDTH:
            WEBC_Append(&carousel_class, " rounded-box");
            break;
        case CAROUSEL_CENTER:
            WEBC_Append(&carousel_class, " rounded-box carousel-center");
            break;
        case CAROUSEL_END:
            WEBC_Append(&carousel_class, " rounded-box carousel-end");
            break;
        case CAROUSEL_FULL_BLEED:
            WEBC_Append(&carousel_class, " rounded-box carousel-center bg-neutral max-w-md space-x-4 p-4");
            break;
        case CAROUSEL_VERTICAL:
            WEBC_Append(&carousel_class, " rounded-box carousel-vertical h-96");
            break;
        case CAROUSEL_NEXT_PREV:
            WEBC_Append(&carousel_class, " w-full");
            break;
        default:
            break;
    }

    WEBC_DivStart(buffer, CLASS(carousel_class));
        for(size_t i = 0; i < item_count; ++i){
            Modifier item_modifier = {
                .class = "carousel-item"
            };


            char* id = NULL;
            if(type == CAROUSEL_FULL_WIDTH){
                item_modifier.class = "carousel-item w-full";
            } else if (type == CAROUSEL_VERTICAL){
                item_modifier.class = "carousel-item h-full";
            } else if(type == CAROUSEL_HALF_WIDTH){
                item_modifier.class = "carousel-item w-1/2";
            } else if(type == CAROUSEL_INDICATOR_BUTTONS){
                item_modifier.class = "carousel-item w-full";

                id = clib_format_text("item%zu", i+1);
                item_modifier.id = id;
            } else if(type == CAROUSEL_NEXT_PREV){
                item_modifier.class = "carousel-item relative w-full";

                id = clib_format_text("slide%zu", i+1);
                item_modifier.id = id;
            }

            WEBC_DivStart(buffer, WEBC_UseModifier(item_modifier));
            if(id != NULL) free(id);
                Modifier img_modifier = {
                    .src = items[i].img_src, 
                    .alt = items[i].img_alt
                };

                if(type == CAROUSEL_FULL_WIDTH || type == CAROUSEL_HALF_WIDTH || type == CAROUSEL_NEXT_PREV){
                    img_modifier.class = "w-full";
                } else if(type == CAROUSEL_FULL_BLEED){
                    img_modifier.class = "rounded-box";
                }                 

                WEBC_Img(buffer, WEBC_UseModifier(img_modifier));

                if(type == CAROUSEL_NEXT_PREV){
                    WEBC_DivStart(buffer, CLASS("absolute left-5 right-5 top-1/2 flex -translate-y-1/2 transform justify-between"));
                        char* prev_href = clib_format_text("#slide%zu", (i == 0) ? item_count : i);
                        char* next_href = clib_format_text("#slide%zu", (i == item_count-1) ? 1 : i+2);
                        Modifier anchor_modifier = {
                            .href = prev_href,
                            .class = "btn btn-circle"
                        };
                        WEBC_Anchor(buffer, WEBC_UseModifier(anchor_modifier), "❮");
                        anchor_modifier.href = next_href;
                        WEBC_Anchor(buffer, WEBC_UseModifier(anchor_modifier), "❯");
                        free(prev_href);
                        free(next_href);
                    WEBC_DivEnd(buffer);
                }
            WEBC_DivEnd(buffer);
        }
    WEBC_DivEnd(buffer);

    if(type == CAROUSEL_INDICATOR_BUTTONS){
        WEBC_DivStart(buffer, CLASS("flex w-full justify-center gap-2 py-2"));
            for(size_t i = 0; i < item_count; ++i){
                char* href = clib_format_text("#item%zu", i+1);
                Modifier anchor_modifier = {
                    .href = href,
                    .class = "btn btn-xs"
                };
                char* num = clib_format_text("%zu", i+1);
                WEBC_Anchor(buffer, WEBC_UseModifier(anchor_modifier), num);
                free(href);
                free(num);
            }
        WEBC_DivEnd(buffer);
    }
    free(carousel_class);
}

