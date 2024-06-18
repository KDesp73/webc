#include "webc-actions.h"
#include "webc-server.h"
#include <stdlib.h>
#include <string.h>


WEBCAPI WebcAction ParseCliArgs(int argc, char** argv)
{
    if(argc == 1){
        WARN("No argument provided.");
        INFO("See %s --help", argv[0]);
        exit(1);
    }

    CliArguments args = clib_make_cli_arguments(8,
        clib_create_argument('h', "help", "Prints this message", no_argument),
        clib_create_argument('v', "version", "Prints the version of the library", no_argument),
        clib_create_argument('e', "export", "Set action as EXPORT", no_argument),
        clib_create_argument('s', "serve-static", "Set action as SERVE_STATIC", no_argument),
        clib_create_argument('d', "serve-dynamic", "Set action as SERVE_DYNAMIC", no_argument),
        clib_create_argument('S', "serve-exported-static", "Set action as SERVE_EXPORTED_STATIC", no_argument),
        clib_create_argument('p', "port", "Set port", required_argument),
        clib_create_argument('P', "print-documents", "Set action as PRINT_DOCUMENTS", no_argument)
    );

    struct option* opts = clib_get_options(args);

    int opt;
    while((opt = getopt_long(argc, argv, clib_generate_cli_format_string(args), opts, NULL)) != -1){
        switch (opt) {
            case 'h':
                clib_cli_help(args, CONCAT(argv[0], " [-h | -v | -s | -d | -S]"), "Made by KDesp73");
                exit(0);
            case 'v':
                printf("webc v%s\n", VERSION);
                exit(0);
            case 'e':
                return ACTION_EXPORT;
            case 's':
                return ACTION_SERVE_STATIC;
            case 'd':
                return ACTION_SERVE_DYNAMIC;
            case 'S':
                return ACTION_SERVE_EXPORTED_STATIC;
            case 'p':
                clib_set_env("WEBC_PORT", optarg, 1);
                break;
            case 'P':
                return ACTION_PRINT_DOCUMENTS;
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
            ExportTree(tree);
            break;
        case ACTION_SERVE_STATIC:
        case ACTION_SERVE_DYNAMIC:
        case ACTION_SERVE_EXPORTED_STATIC: {
            Cstr port_env = clib_get_env("WEBC_PORT");
            if(port_env == NULL){
                PANIC("WEBC_PORT environment variable is not set");
            }

            if(action == ACTION_SERVE_STATIC){
                ServeTree(atoi(port_env), tree);     
            } else if(action == ACTION_SERVE_DYNAMIC){
                PANIC("ACTION_SERVE_DYNAMIC is not implemented yet");
            } else if(action == ACTION_SERVE_EXPORTED_STATIC){
                ServeExported(atoi(port_env), tree.root);
            }
            break;
        }
        case ACTION_PRINT_DOCUMENTS:
            for (size_t i = 0; i < tree.count; ++i) {
                printf("%s%s%s\n", COLOR_FG(i+1), tree.routes[i]->path, RESET);
                printf("%s\n\n", tree.routes[i]->buffer);
            }
            break;
        default:
            PANIC("Unknown Action");
    }
}

WEBCAPI Route* MakeRoute(Cstr path, char* buffer)
{
    Route* route = (Route*) malloc(sizeof(Route));
    route->path = (char*) malloc(sizeof(char) * (strlen(path) + 1));
    strcpy(route->path, path);
    route->buffer = (char*) malloc(sizeof(char) * (strlen(buffer) + 1));
    strcpy(route->buffer, buffer);

    return route;
}

// TODO: dont include the root again in every path
WEBCAPI Tree MakeTree(Cstr root, Route* first, ...)
{
    Tree result = {0};
    result.root = root;
    result.count = 0;
    if(first == NULL) return result;

    va_list args;
    va_start(args, first);
    result.count++;
    for (Route* next = va_arg(args, Route*); next != NULL; next = va_arg(args, Route*)) {
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
