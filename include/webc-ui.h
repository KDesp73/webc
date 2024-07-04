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
 * @author ThanasisGeorg (Thanasis Georgalis)
 * @version 0.0.0
 */

#ifndef WEBCUI_H
#define WEBCUI_H

// ############ Includes  ############ //

#include "webc-core.h"

#ifdef __cplusplus
extern "C" {
#endif

// ############ Types ############ //

typedef enum {
    AVATAR_DEFAULT,
    AVATAR_CIRCLE,
    AVATAR_RING,
    AVATAR_PRESENCE_ONLINE,
    AVATAR_PRESENCE_OFFLINE
} AvatarType;

typedef struct {
    Cstr title;
    Cstr content;
} Accordion;

// ############ UI Elements ############ //

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

WEBCAPI void WEBC_UIButton(char** buffer, Modifier modifier, Cstr text);

// #ifdef DAISY_UI

/**
 * https://daisyui.com/components/accordion/#accordion-using-radio-inputs
 */
WEBCAPI void WEBC_DaisyAccordion(char** buffer, Accordion accordion, int open);

/**
 * https://daisyui.com/components/avatar/#avatar-in-custom-sizes 
 */
WEBCAPI void WEBC_DaisyAvatar(char** buffer, AvatarType type, Cstr img_src, Cstr placeholder, size_t width);

// #endif // DAISY_UI

#ifdef __cplusplus
}
#endif
#endif // WEBCUI_H

