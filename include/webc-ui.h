/**
 * MIT License
 * 
 * Copyright (c) 2024 Konstantinos Despoinidis (and other contributors)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file webc-ui.h
 * @author KDesp73 (Konstantinos Despoinidis)
 * @version 0.0.1
 */

#ifndef WEBCUI_H
#define WEBCUI_H

// ############ Includes  ############ //

#include "webc-core.h"

#ifdef __cplusplus
extern "C" {
#endif

// ############ Types ############ //

typedef struct {
    Cstr title;
    Cstr content;
} Accordion;

typedef enum {
    AVATAR_DEFAULT,
    AVATAR_CIRCLE,
    AVATAR_RING,
    AVATAR_PRESENCE_ONLINE,
    AVATAR_PRESENCE_OFFLINE
} AvatarType;

typedef enum {
    BADGE_DEFAULT,
    BADGE_OUTLINE
} BadgeType;

typedef enum {
    CAROUSEL_DEFAULT,
    CAROUSEL_CENTER,
    CAROUSEL_END,
    CAROUSEL_FULL_WIDTH,
    CAROUSEL_HALF_WIDTH,
    CAROUSEL_FULL_BLEED,
    CAROUSEL_INDICATOR_BUTTONS,
    CAROUSEL_NEXT_PREV,
    CAROUSEL_VERTICAL
} CarouselType;

typedef struct {
    Cstr img_src;
    Cstr img_alt;
} CarouselItem;

typedef enum {
    CHAT_BUBBLE_DEFAULT,
    CHAT_BUBBLE_IMAGE,
    CHAT_BUBBLE_FULL,
    CHAT_BUBBLE_HEADER_FOOTER
} ChatBubbleType;

typedef struct {
    Cstr text;
    Cstr color;
    Cstr img_src;
    Cstr img_alt;
    Cstr name;
    Cstr time;
    Cstr footer;
} ChatBubble;

typedef enum {
    ALERT_DEFAULT = 0,
    ALERT_INFO = 1,
    ALERT_SUCCESS = 2,
    ALERT_WARNING = 3,
    ALERT_ERROR = 4,
} AlertType;

typedef struct {
    Cstr text;
    Cstr color;
    Cstr svg;
    Cstr onclick;
} Button;

typedef enum {
    BUTTON_DEFAULT,
    BUTTON_OUTLINE,
    BUTTON_RESPONSIVE,
    BUTTON_WIDE,
    BUTTON_GLASS,
    BUTTON_SQUARE,
    BUTTON_CIRCLE,
    BUTTON_ICON,
    BUTTON_LOADING,
    BUTTON_LOADING_TEXT
} ButtonType;

// ############ UI Elements ############ //

// #ifdef DAISY_UI

#define DAISYUI_LINK \
    WEBC_MakeTag(LINK, \
            WEBC_MakeAttributeList( \
                WEBC_MakeAttribute(ATTR_HREF, "https://cdn.jsdelivr.net/npm/daisyui@4.12.10/dist/full.min.css"), \
                WEBC_MakeAttribute(ATTR_REL, "stylesheet"), \
                WEBC_MakeAttribute(ATTR_TYPE, "text/css"), \
                NULL \
            ) \
        )

#define TAILWINDCSS_SCRIPT(buffer) \
    WEBC_PlainText(buffer, "<script src=\"https://cdn.tailwindcss.com\"></script>");


/**
 * https://daisyui.com/components/accordion/
 */
WEBCAPI void WEBC_DaisyAccordion(char** buffer, Accordion accordion, int open);

/**
 * https://daisyui.com/components/avatar/
 */
WEBCAPI void WEBC_DaisyAvatar(char** buffer, AvatarType type, Cstr img_src, Cstr placeholder, size_t width);


/**
 * https://daisyui.com/components/badge/
 *
 * See colors at: https://daisyui.com/docs/colors/
 *
 * Example: WEBC_DaisyBadge(&buffer, BADGE_DEFAULT, "primary", "Example");
 */
WEBCAPI void WEBC_DaisyBadge(char** buffer, BadgeType type, Cstr color, Cstr text);

// TODO: Card

/**
 * https://daisyui.com/components/carousel/
 *
 * Set width to 0 for full width
 */
WEBCAPI void WEBC_DaisyCarousel(char** buffer, CarouselType type, size_t width, CarouselItem items[], size_t item_count);

/**
 * https://daisyui.com/components/chat/
 */
WEBCAPI void WEBC_DaisyChatBubble(char** buffer, ChatBubbleType type, ChatBubble bubble, int left);

/**
 * https://daisyui.com/components/collapse/
 */
WEBCAPI void WEBC_DaisyCollapse(char** buffer, Cstr title, Cstr contents);

/**
 * https://daisyui.com.components/alert/
 */
WEBCAPI void WEBC_DaisyAlert(char** buffer, AlertType type, Cstr text);

/**
 * https://daisyui.com/components/button/
 */
WEBCAPI void WEBC_DaisyButton(char** buffer, ButtonType type, Button button);

// #endif // DAISY_UI

#ifdef __cplusplus
}
#endif

#endif // WEBCUI_H

