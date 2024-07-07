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
 * @version 0.0.3
 */

#ifndef WEBCUI_H
#define WEBCUI_H

// ############ Includes  ############ //

#include "webc-core.h"

#ifdef __cplusplus
extern "C" {
#endif

// ############ Colors ############ //

#define DAISY_COLOR_PRIMARY "primary"
#define DAISY_COLOR_PRIMARY_CONTENT "primary-content"
#define DAISY_COLOR_SECONDARY "secondary"
#define DAISY_COLOR_SECONDARY_CONTENT "secondary-content"
#define DAISY_COLOR_ACCENT "accent"
#define DAISY_COLOR_ACCENT_CONTENT "accent-content"
#define DAISY_COLOR_NEUTRAL "neutral"
#define DAISY_COLOR_NEUTRAL_CONTENT "neutral-content"
#define DAISY_COLOR_BASE_100 "base-100"
#define DAISY_COLOR_BASE_200 "base-200"
#define DAISY_COLOR_BASE_300 "base-300"
#define DAISY_COLOR_INFO "info"
#define DAISY_COLOR_INFO_CONTENT "info-content"
#define DAISY_COLOR_SUCCESS "success"
#define DAISY_COLOR_SUCCESS_CONTENT "success-content"
#define DAISY_COLOR_WARNING "warning"
#define DAISY_COLOR_WARNING_CONTENT "warning-content"
#define DAISY_COLOR_ERROR "error"
#define DAISY_COLOR_ERROR_CONTENT "error-content"

#define DAISY_SIZE_XS "xs"
#define DAISY_SIZE_SM "sm"
#define DAISY_SIZE_MD "md"
#define DAISY_SIZE_LG "lg"


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
    MouseEvents events;
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

typedef struct {
    Cstr title;
    Cstr link;
    Cstr target;
} NavLink;

typedef enum {
    LOADING_SPINNER,
    LOADING_DOTS,
    LOADING_RING,
    LOADING_BALL,
    LOADING_BAR,
    LOADING_INFINITY
} LoadingType;

typedef struct {
    Cstr prefix;
    Cstr code;
} CodeLine;

// ############ Utils ############ //

#define SECURE_STR(s) ((s == NULL) ? "NULL" : s)
int is_valid_daisy_size(Cstr size);
int is_valid_daisy_color(Cstr color);
void append_class_attr(char** class, Cstr attr);

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
 * https://daisyui.com/components/alert/
 */
WEBCAPI void WEBC_DaisyAlert(char** buffer, AlertType type, Cstr text);

/**
 * https://daisyui.com/components/button/
 */
WEBCAPI void WEBC_DaisyButton(char** buffer, ButtonType type, Button button);

/**
 * https://daisyui.com/components/checkbox/
 *
 * Set color to NULL to use default color
 */
WEBCAPI void WEBC_DaisyCheckbox(char** buffer, Cstr label, Cstr color);

/**
 * https://daisyui.com/components/drawer/
 *
 * Set sidebar_items to NULL or sidebar_items_counte to 0, to use navbar_content in both cases
 */
WEBCAPI void WEBC_DaisyNavbarSidebar(
        char** buffer, 
        Cstr title, 
        NavLink navbar_items[],
        size_t navbar_items_count,
        NavLink sidebar_items[],
        size_t sidebar_items_count,
        BlockContents page_content);

/**
 * https://daisyui.com/components/dropdown/
 */
WEBCAPI void WEBC_DaisyDropdown(char** buffer, Cstr title, NavLink items[], size_t count);

/**
 * https://daisyui.com/components/loading/
 *
 * @param size [xs | sm | md | lg]
 */
WEBCAPI void WEBC_DaisyLoading(char** buffer, LoadingType type, Cstr size);

/**
 * https://daisyui.com/components/file-input/
 */
WEBCAPI void WEBC_DaisyFileInput(char** buffer, Cstr color);

/**
 * https://daisyui.com/components/indicator/
 */
WEBCAPI void WEBC_DaisyIndicator(char** buffer, Cstr text, BlockContents toplaceon);

/**
 * https://daisyui.com/components/input/
 *
 * @param icon The svg html (viewBox should be "0 0 16 16")
 */
WEBCAPI void WEBC_DaisyTextInput(char** buffer, int password, Cstr placeholder, Cstr icon);

/**
 * https://daisyui.com/components/mockup-code/
 */
WEBCAPI void WEBC_DaisyCode(char** buffer, Cstr color, CodeLine code[], size_t count);

/**
 * https://daisyui.com/components/modal/
 *
 * call <id>.showModal() to open the dialog
 * call <id>.close() to close it
 * In case if NULL id, id is set to "my_modal"
 */
WEBCAPI void WEBC_DaisyModal(char** buffer, Cstr id, Cstr title, Cstr content);

/**
 * https://daisyui.com/components/pagination/
 *
 * @param from Lower limit
 * @param to Upper limit (non inclusive)
 * @param checked Initial active index (from <= checked < to)
 */
WEBCAPI void WEBC_DaisyPagination(char** buffer, size_t from, size_t to, size_t checked);

/**
 * https://daisyui.com/components/progress/
 *
 * Set max to 0 to enable indeterminate progress
 */
WEBCAPI void WEBC_DaisyProgress(char** buffer, size_t width, Cstr color, size_t value, size_t max);

/**
 * https://daisyui.com/components/radio/
 */
WEBCAPI void WEBC_DaisyRadio(char** buffer, Cstr group_name, Cstr color, int checked);

/**
 * https://daisyui.com/components/range/
 *
 * Set step to 0 for smooth range
 * 
 * @param size [xs | sm | md | lg]
 */
WEBCAPI void WEBC_DaisyRange(
        char** buffer, 
        size_t min, 
        size_t max, 
        size_t value, 
        size_t step, 
        Cstr size, 
        Cstr color);

/**
 * https://daisyui.com/components/select/
 */
WEBCAPI void WEBC_DaisySelect(char** buffer, Cstr title, char* options[], size_t count, Cstr color);

/**
 * https://daisyui.com/components/textarea/
 *
 * @param size [xs | sm | md | lg]
 */
WEBCAPI void WEBC_DaisyTextarea(char** buffer, Cstr placeholder, Cstr color, Cstr size);

/**
 * https://daisyui.com/components/theme-controller/
 */
WEBCAPI void WEBC_DaisyThemeController(char** buffer);

/**
 * https://daisyui.com/components/toast/
 */
WEBCAPI void WEBC_DaisyToast(char** buffer, Cstr text, Cstr color);

/**
 * https://daisyui.com/components/toggle/
 */
WEBCAPI void WEBC_DaisyToggle(char** buffer, int checked, Cstr color, Cstr size);

/**
 * https://daisyui.com/components/tooltip/
 */
WEBCAPI void WEBC_DaisyTooltip(char** buffer, Cstr tooltip, Cstr location, int open, Cstr color, BlockContents toplaceon);

// #endif // DAISY_UI

#ifdef __cplusplus
}
#endif

#endif // WEBCUI_H

