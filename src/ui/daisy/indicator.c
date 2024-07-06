#include "webc-core.h"
#include "webc-ui.h"


WEBCAPI void WEBC_DaisyIndicator(char** buffer, Cstr text, BlockContents toplaceon)
{
    WEBC_DivStart(buffer, CLASS("indicator"));
        WEBC_Span(buffer, CLASS("indicator-item badge badge-primary"), text);
        toplaceon(buffer);
    WEBC_DivEnd(buffer);
}
