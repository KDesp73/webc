#include "webc-core.h"
#include "webc-ui.h"

char* svgs[] = {
    "<svg\n"
        "xmlns=\"http://www.w3.org/2000/svg\"\n"
        "fill=\"none\"\n"
        "viewBox=\"0 0 24 24\"\n"
        "class=\"stroke-info h-6 w-6 shrink-0\">\n"
        "<path\n"
        "stroke-linecap=\"round\"\n"
        "stroke-linejoin=\"round\"\n"
        "stroke-width=\"2\"\n"
        "d=\"M13 16h-1v-4h-1m1-4h.01M21 12a9 9 0 11-18 0 9 9 0 0118 0z\"></path>\n"
        "</svg>",
    "<svg\n"
        "xmlns=\"http://www.w3.org/2000/svg\"\n"
        "fill=\"none\"\n"
        "viewBox=\"0 0 24 24\"\n"
        "class=\"h-6 w-6 shrink-0 stroke-current\">\n"
        "<path\n"
        "stroke-linecap=\"round\"\n"
        "stroke-linejoin=\"round\"\n"
        "stroke-width=\"2\"\n"
        "d=\"M13 16h-1v-4h-1m1-4h.01M21 12a9 9 0 11-18 0 9 9 0 0118 0z\"></path>\n"
        "</svg>",
    "<svg\n"
        "xmlns=\"http://www.w3.org/2000/svg\"\n"
        "class=\"h-6 w-6 shrink-0 stroke-current\"\n"
        "fill=\"none\"\n"
        "viewBox=\"0 0 24 24\">\n"
        "<path\n"
        "stroke-linecap=\"round\"\n"
        "stroke-linejoin=\"round\"\n"
        "stroke-width=\"2\"\n"
        "d=\"M9 12l2 2 4-4m6 2a9 9 0 11-18 0 9 9 0 0118 0z\" />\n"
        "</svg>",
    "<svg\n"
        "xmlns=\"http://www.w3.org/2000/svg\"\n"
        "class=\"h-6 w-6 shrink-0 stroke-current\"\n"
        "fill=\"none\"\n"
        "viewBox=\"0 0 24 24\">\n"
        "<path\n"
        "stroke-linecap=\"round\"\n"
        "stroke-linejoin=\"round\"\n"
        "stroke-width=\"2\"\n"
        "d=\"M12 9v2m0 4h.01m-6.938 4h13.856c1.54 0 2.502-1.667 1.732-3L13.732 4c-.77-1.333-2.694-1.333-3.464 0L3.34 16c-.77 1.333.192 3 1.732 3z\" />\n"
        "</svg>",
    "<svg\n"
        "xmlns=\"http://www.w3.org/2000/svg\"\n"
        "class=\"h-6 w-6 shrink-0 stroke-current\"\n"
        "fill=\"none\"\n"
        "viewBox=\"0 0 24 24\">\n"
        "<path\n"
        "stroke-linecap=\"round\"\n"
        "stroke-linejoin=\"round\"\n"
        "stroke-width=\"2\"\n"
        "d=\"M10 14l2-2m0 0l2-2m-2 2l-2-2m2 2l2 2m7-2a9 9 0 11-18 0 9 9 0 0118 0z\" />\n"
        "</svg>"
};

WEBCAPI void WEBC_DaisyAlert(char** buffer, AlertType type, Cstr text)
{
    char* alert_class = NULL;
    char* svg = NULL;

    switch (type) {
        case ALERT_DEFAULT:
            alert_class = "alert";
            svg = svgs[ALERT_DEFAULT];
            break;
        case ALERT_INFO:
            alert_class = "alert alert-info";
            svg = svgs[ALERT_INFO];
            break;
        case ALERT_SUCCESS:
            alert_class = "alert alert-success";
            svg = svgs[ALERT_SUCCESS];
            break;
        case ALERT_WARNING:
            alert_class = "alert alert-warning";
            svg = svgs[ALERT_WARNING];
            break;
        case ALERT_ERROR:
            alert_class = "alert alert-error";
            svg = svgs[ALERT_ERROR];
            break;
        default:
            ERRO("Unknown Alert type: %d", type);
            return;
    }

    WEBC_DivStart(buffer, WEBC_UseModifier((Modifier) {.role = "alert", .class = alert_class}));
        WEBC_PlainText(buffer, svg);
        WEBC_Span(buffer, NO_ATTRIBUTES, (text == NULL) ? "" : text);
    WEBC_DivEnd(buffer);
}
