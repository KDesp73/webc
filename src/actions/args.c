#include "webc-actions.h"
#include <stdlib.h>
#include <string.h>


WEBCAPI WebcAction ParseCliArgs(int argc, char** argv)
{
    if(argc == 1){
        WARN("No argument provided.");
        INFO("See %s --help", argv[0]);
        exit(1);
    }

    CliArguments args = clib_make_cli_arguments(6,
        clib_create_argument('h', "help", "Prints this message", no_argument),
        clib_create_argument('v', "version", "Prints the version of the library", no_argument),
        clib_create_argument('e', "export", "Set action as EXPORT", no_argument),
        clib_create_argument('s', "serve-static", "Set action as SERVE_STATIC", no_argument),
        clib_create_argument('d', "serve-dynamic", "Set action as SERVE_DYNAMIC", no_argument),
        clib_create_argument('S', "serve-exported-static", "Set action as SERVE_EXPORTED_STATIC", no_argument)
    );

    struct option* opts = clib_get_options(args);

    int opt;
    while((opt = getopt_long(argc, argv, clib_generate_cli_format_string(args), opts, NULL)) != -1){
        switch (opt) {
            case 'h':
                clib_cli_help(args, CONCAT(argv[0], " [-h | -v | -s | -d | -S]"), "Made by KDesp73");
                exit(0);
            case 'v':
                printf("webc v%s", VERSION);
                exit(0);
            case 'e':
                return ACTION_EXPORT;
            case 's':
                return ACTION_SERVE_STATIC;
            case 'd':
                return ACTION_SERVE_DYNAMIC;
            case 'S':
                return ACTION_SERVE_EXPORTED_STATIC;
            default:
                exit(1);
        }

    }
    return -1;
}

WEBCAPI void HandleAction(WebcAction action, Tree tree)
{
    switch (action) {
        case ACTION_EXPORT:
            for (size_t i = 0; i < tree.count; ++i) {
                ExportRoute(*tree.routes[i]);
            }
            break;
        case ACTION_SERVE_STATIC:
            PANIC("ACTION_SERVE_STATIC not implemented yet");
        case ACTION_SERVE_DYNAMIC:
            PANIC("ACTION_SERVE_DYNAMIC not implemented yet");
        case ACTION_SERVE_EXPORTED_STATIC:
            PANIC("ACTION_SERVE_EXPORTED_STATIC not implemented yet");
        default:
            PANIC("Unknown Action");
    }
}

WEBCAPI Route* MakeRoute(Cstr path, char* buffer)
{
    Route* route = (Route*) malloc(sizeof(Route));
    route->path = (char*) malloc(sizeof(char) * strlen(path));
    strcpy(route->path, path);
    route->buffer = (char*) malloc(sizeof(char) * strlen(buffer));
    strcpy(route->buffer, buffer);

    return route;
}

WEBCAPI Tree MakeTree(Route* first, ...)
{
    Tree result = {0};
    result.count = 0;
    if(first == NULL) return result;

    va_list args;
    va_start(args, first);
    result.count++;
    for (Attribute* next = va_arg(args, Attribute*); next != NULL; next = va_arg(args, Attribute*)) {
        result.count++;
    }
    va_end(args);

    result.routes = (Route**) malloc(sizeof(result.routes[0]) * result.count);
    if (result.routes == NULL) {
        PANIC("could not allocate memory: %s", strerror(errno));
    }

    va_start(args, first);
    result.routes[0] = first;
    for (size_t i = 1; i < result.count; ++i) {
        Route* next = va_arg(args, Route*);
        result.routes[i]= next;
    }
    va_end(args);

    return result; 
    
}
