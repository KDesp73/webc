# Examples

## Simple Portfolio

Check it out [here](https://github.com/KDesp73/webc-portfolio)

## Simple Usage

```c
#include "webc-core.h"

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

## Advanced Usage

```c
#include "webc-core.h"
#include "webc-actions.h"

char* Index()
{
    char* buffer = NULL;
    /* Build the Page */
    return buffer;
}

char* About()
{
    char* buffer = NULL;
    /* Build the Page */
    return buffer;
}

char* Contact()
{
    char* buffer = NULL;
    /* Build the Page */
    return buffer;
}

int main(int argc, char** argv)
{
    WebcAction action = ParseCliArgs(argc, argv);

    Cstr root = "site";
    Tree tree = MakeTree(root,
        MakeRoute("/", Index()),
        MakeRoute("/about", About()),
        MakeRoute("/contact", Contact()),
        NULL
    );

    HandleAction(action, tree);
    return 0;
}
```
Structuring your code in this way adds these options to your executable

```
-h --help                       Prints this message [no argument]
-v --version                    Prints the version of the library [no argument]
-e --export                     Set action as EXPORT [no argument]
-s --serve-static               Set action as SERVE_STATIC [no argument]
-d --serve-dynamic              Set action as SERVE_DYNAMIC [no argument]
-S --serve-exported-static      Set action as SERVE_EXPORTED_STATIC [no argument]
-p --port                       Set port [requires argument]
-r --root                       Set root [requires argument]
```

### Example option usage

- Export the site and serve it on port 8080
    ```console
    webc-site -eS -p8080
    ```

- Serve the site virtually (without having exported the files)
    ```console
    webc-site -s -p8080
    ```

- Serve any static site
    ```console
    webc-site -S -rpath/to/site -p8080
    ```
