#include "webc-core.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

WEBCAPI Attribute* WEBC_MakeAttribute(AttributeName name, const char* value)
{
    Attribute* attr = (Attribute*)malloc(sizeof(Attribute));
    if (attr == NULL) {
        return NULL;
    }

    attr->name = name;

    if(value == NULL) {
        attr->value = NULL;
        return attr;
    }

    attr->value = (char*)malloc((strlen(value) + 1));
    memset(attr->value, 0, strlen(value) + 1);

    if (attr->value == NULL) {
        free(attr);
        return NULL;
    }

    strcpy(attr->value, value);
    attr->value[strlen(value)] = '\0';

    return attr;
}

WEBCAPI Cstr WEBC_AttributeNameToString(AttributeName attr)
{
    switch (attr) {
        case ATTR_ACTION:
            return "action";
        case ATTR_ALT:
            return "alt";
        case ATTR_CHARSET:
            return "charset";
        case ATTR_CLASS:
            return "class";
        case ATTR_CONTENT:
            return "content";
        case ATTR_HEIGHT:
            return "height";
        case ATTR_HREF:
            return "href";
        case ATTR_HTTP_EQUIV:
            return "http_equiv";
        case ATTR_ID:
            return "id";
        case ATTR_METHOD:
            return "method";
        case ATTR_NAME:
            return "name";
        case ATTR_PROPERTY:
            return "property";
        case ATTR_REL:
            return "rel";
        case ATTR_SRC:
            return "src";
        case ATTR_STYLE:
            return "style";
        case ATTR_TABINDEX:
            return "tabindex";
        case ATTR_TARGET:
            return "target";
        case ATTR_TITLE:
            return "title";
        case ATTR_TYPE:
            return "type";
        case ATTR_VALUE:
            return "value";
        case ATTR_CHECKED:
            return "checked";
        case ATTR_WIDTH:
            return "width";
        case ATTR_ROLE:
            return "role";
        case ATTR_ONCLICK:
            return "onclick";
        case ATTR_ONCONTEXTMENU:
            return "oncontextmenu";
        case ATTR_ONDBLCLICK:
            return "ondblclick";
        case ATTR_ONMOUSEDOWN:
            return "onmousedown";
        case ATTR_ONMOUSEENTER:
            return "onmouseenter";
        case ATTR_ONMOUSELEAVE:
            return "onmouseleave";
        case ATTR_ONMOUSEMOVE:
            return "onmousemove";
        case ATTR_ONMOUSEOUT:
            return "onmouseout";
        case ATTR_ONMOUSEOVER:
            return "onmouseover";
        case ATTR_ONMOUSEUP:
            return "onmouseup";
        case ATTR_FOR:
            return "for";
        case ATTR_ARIA_LABEL:
            return "aria-label";
        case ATTR_PLACEHOLDER:
            return "placeholder";
        case ATTR_DATA_PREFIX:
            return "data-prefix";
        case ATTR_DATA_TIP:
            return "data-tip";
        case ATTR_MAX:
            return "max";
        case ATTR_MIN:
            return "min";
        case ATTR_STEP:
            return "step";
        case ATTR_DISABLED:
            return "disabled";
        case ATTR_SELECTED:
            return "selected";
        default:
            break;
    }
    return NULL;
}

WEBCAPI void WEBC_CleanAttributeList(AttributeList list)
{
    for(size_t i = 0; i < list.count; ++i){
        if(list.items != NULL){
            // free(list.items[i]->value);
            free(list.items[i]);
        }
    }
    free(list.items);
}

WEBCAPI AttributeList WEBC_MakeAttributeList(Attribute* first, ...)
{
    AttributeList result = {0};
    result.count = 0;
    if(first == NULL) return result;

    va_list args;
    va_start(args, first);
    result.count++;
    for (Attribute* next = va_arg(args, Attribute*); next != NULL; next = va_arg(args, Attribute*)) {
        result.count++;
    }
    va_end(args);

    result.items = (Attribute**) malloc(sizeof(result.items[0]) * result.count);
    if (result.items == NULL) {
        PANIC("could not allocate memory: %s", strerror(errno));
    }

    va_start(args, first);
    result.items[0] = first;
    for (size_t i = 1; i < result.count; ++i) {
        Attribute* next = va_arg(args, Attribute*);
        result.items[i]= next;
    }
    va_end(args);

    return result; 
}


WEBCAPI AttributeList WEBC_UseModifier(Modifier modifier)
{
    AttributeList list = {0};

    // Count the number of set attributes
    if (modifier.style != NULL) list.count++;
    if (modifier.href != NULL) list.count++;
    if (modifier.id != NULL) list.count++;
    if (modifier.class != NULL) list.count++;
    if (modifier.target != NULL) list.count++;
    if (modifier.src != NULL) list.count++;
    if (modifier.alt != NULL) list.count++;
    if (modifier.width > 0) list.count++;
    if (modifier.height > 0) list.count++;
    if (modifier.type != NULL) list.count++;
    if (modifier.action != NULL) list.count++;
    if (modifier.method != NULL) list.count++;
    if (modifier.value != NULL) list.count++;
    if (modifier.name != NULL) list.count++;
    if (modifier.checked != NULL) list.count++;
    if (modifier.tabindex != NULL) list.count++;
    if (modifier.role != NULL) list.count++;
    if (modifier.mouse_events.onclick != NULL) list.count++;
    if (modifier.mouse_events.onmouseup != NULL) list.count++;
    if (modifier.mouse_events.onclick != NULL) list.count++;
    if (modifier.mouse_events.oncontextmenu != NULL) list.count++;
    if (modifier.mouse_events.ondblclick != NULL) list.count++;
    if (modifier.mouse_events.onmousedown != NULL) list.count++;
    if (modifier.mouse_events.onmouseenter != NULL) list.count++;
    if (modifier.mouse_events.onmouseleave != NULL) list.count++;
    if (modifier.mouse_events.onmousemove != NULL) list.count++;
    if (modifier.mouse_events.onmouseout != NULL) list.count++;
    if (modifier.mouse_events.onmouseover != NULL) list.count++;
    if (modifier.for_ != NULL) list.count++;
    if (modifier.aria_label != NULL) list.count++;
    if (modifier.placeholder != NULL) list.count++;
    if (modifier.data_prefix != NULL) list.count++;
    if (modifier.data_tip != NULL) list.count++;
    if (modifier.max != NULL) list.count++;
    if (modifier.min != NULL) list.count++;
    if (modifier.step != NULL) list.count++;
    if (modifier.disabled > 0) list.count++;
    if(modifier.selected > 0) list.count++;


    list.items = (Attribute**) malloc(sizeof(list.items[0]) * list.count);
    list.count = 0;

    if(modifier.style != NULL)
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_STYLE, modifier.style);

    if(modifier.href != NULL)
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_HREF, modifier.href);
    
    if(modifier.id != NULL)
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_ID, modifier.id);
    
    if(modifier.class != NULL)
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_CLASS, modifier.class);
    
    if(modifier.target != NULL)
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_TARGET, modifier.target);

    if(modifier.src != NULL)
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_SRC, modifier.src);

    if(modifier.alt != NULL)
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_ALT, modifier.alt);

    if(modifier.width > 0){
        char* width = clib_format_text("%zu", modifier.width);
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_WIDTH, width);
        free(width);
    }

    if(modifier.height > 0){
        char* height = clib_format_text("%zu", modifier.height);
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_HEIGHT, height);
        free(height);
    }

    if (modifier.type != NULL)
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_TYPE,  modifier.type);

    if (modifier.action != NULL)
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_ACTION,  modifier.action);

    if (modifier.method != NULL)
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_METHOD,  modifier.method);

    if (modifier.value != NULL)
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_VALUE,  modifier.value);

    if (modifier.name != NULL)
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_NAME,  modifier.name);

    if (modifier.checked != NULL)
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_CHECKED,  modifier.checked);

    if (modifier.tabindex != NULL)
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_TABINDEX,  modifier.tabindex);

    if (modifier.role != NULL)
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_ROLE,  modifier.role);

    if (modifier.for_ != NULL)
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_FOR,  modifier.for_);

    if (modifier.aria_label != NULL)
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_ARIA_LABEL,  modifier.aria_label);

    if (modifier.placeholder != NULL)
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_PLACEHOLDER,  modifier.placeholder);

    if (modifier.data_prefix != NULL)
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_DATA_PREFIX,  modifier.data_prefix);

    if (modifier.data_tip != NULL)
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_DATA_TIP,  modifier.data_tip);

    if (modifier.max != NULL)
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_MAX,  modifier.max);

    if (modifier.min != NULL)
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_MIN,  modifier.min);

    if (modifier.step != NULL)
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_STEP,  modifier.step);

    if(modifier.disabled > 0){
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_DISABLED, NULL);
    }
    if(modifier.selected > 0){
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_SELECTED, NULL);
    }


    if(modifier.mouse_events.onclick != NULL)
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_ONCLICK, modifier.mouse_events.onclick);
    if(modifier.mouse_events.oncontextmenu != NULL)
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_ONCONTEXTMENU, modifier.mouse_events.oncontextmenu);
    if(modifier.mouse_events.ondblclick != NULL)
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_ONDBLCLICK, modifier.mouse_events.ondblclick);
    if(modifier.mouse_events.onmousedown != NULL)
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_ONMOUSEDOWN, modifier.mouse_events.onmousedown);
    if(modifier.mouse_events.onmouseenter != NULL)
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_ONMOUSEENTER, modifier.mouse_events.onmouseenter);
    if(modifier.mouse_events.onmouseleave != NULL)
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_ONMOUSELEAVE, modifier.mouse_events.onmouseleave);
    if(modifier.mouse_events.onmousemove != NULL)
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_ONMOUSEMOVE, modifier.mouse_events.onmousemove);
    if(modifier.mouse_events.onmouseout != NULL)
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_ONMOUSEOUT, modifier.mouse_events.onmouseout);
    if(modifier.mouse_events.onmouseover != NULL)
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_ONMOUSEOVER, modifier.mouse_events.onmouseover);
    if(modifier.mouse_events.onmouseup != NULL)
        list.items[list.count++] = WEBC_MakeAttribute(ATTR_ONMOUSEUP, modifier.mouse_events.onmouseup);
    return list;
}
