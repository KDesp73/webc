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
int main(void)
{
    Cstr output = "index.html";
    char* file = NULL;

    HtmlInit(&file, "en");
    Header(&file, "WebC-Example",
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

    BodyStart(&file);
        for(size_t i = 1; i <= 6; ++i){
            Heading(&file, i, clib_format_text("Heading %zu", i));
        }
        Paragraph(&file, "Hello from C");
    BodyEnd(&file);

    Export(file, output);
    Clean(&file);
    return 0;
}
```

## Documentation

See [webc.h](./include/webc.h)

## LICENSE

[MIT](./LICENSE)
