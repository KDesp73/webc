# webc

Write websites using the far superior (and actual programming language) C. 

<!--toc:start-->
- [webc](#webc)
  - [Example](#example)
  - [Documentation](#documentation)
  - [LICENSE](#license)
<!--toc:end-->

## Example

```c
#include "webc.h"

int main(void)
{
    char* buffer = NULL;

    HtmlInit(&buffer, "en");
    Header(&buffer, "WebC-Example",
        MakeTag("meta",
            MakeAttribute(ATTR_NAME, "author"),
            MakeAttribute(ATTR_CONTENT, "Konstantinos Despoinidis"),
            NULL
        ),
        MakeTag("link",
            MakeAttribute(ATTR_HREF, "./style.css"),
            NULL
        ),
        NULL
    );

    BodyStart(&buffer);
        for(size_t i = 1; i <= 6; ++i){
            Heading(&buffer, i, clib_format_text("Heading %zu", i));
        }
        Paragraph(&buffer, "Hello from C");
    BodyEnd(&buffer);

    Cstr root = "site/"
    Route index = {
        .path = root,
        .buffer = buffer
    };

    ExportRoute(index);
    Clean(&buffer);
    return 0;
}
```

## Documentation

See [webc.h](./include/webc.h)

## LICENSE

[MIT](./LICENSE)
