# webc

Write websites using the far superior (and actual programming language) C. 

## Example

```c
int main(void)
{
    Cstr output = "index.html";
    char* file = NULL;

    HtmlInit(&file, "en");
    Header(&file, "WebC-Example",
        MakeTag("meta",
            MakeAttribute(NAME, "author"),
            MakeAttribute(CONTENT, "Konstantinos Despoinidis"),
            NULL
        ),
        MakeTag("link",
            MakeAttribute(HREF, "./style.css"),
            NULL
        ),
        NULL
    );

    BodyStart(&file);
        Heading(&file, 1, "Heading 1");
        Heading(&file, 2, "Heading 2");
        Heading(&file, 3, "Heading 3");
        Heading(&file, 4, "Heading 4");
        Heading(&file, 5, "Heading 5");
        Heading(&file, 6, "Heading 6");
        Paragraph(&file, "Hello from C");
    BodyEnd(&file);

    Export(file, output);
    Clean(file);
    return 0;
}
```

## LICENSE

[MIT](./LICENSE)
