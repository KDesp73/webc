#include "webc-actions.h"
#include "webc-server.h"
#include <signal.h>
#include <stdlib.h>
#include <string.h>


WEBCAPI WebcAction ParseCliArgs(int argc, char** argv)
{
    if(argc == 1){
        WARN("No argument provided.");
        INFO("See %s --help", argv[0]);
        exit(1);
    }

    WebcAction action = {0};

    CliArguments args = clib_make_cli_arguments(8,
        clib_create_argument('h', "help", "Prints this message", no_argument),
        clib_create_argument('v', "version", "Prints the version of the library", no_argument),
        clib_create_argument('e', "export", "Set action as EXPORT", no_argument),
        clib_create_argument('s', "serve-static", "Set action as SERVE_STATIC", no_argument),
        clib_create_argument('d', "serve-dynamic", "Set action as SERVE_DYNAMIC", no_argument),
        clib_create_argument('S', "serve-exported-static", "Set action as SERVE_EXPORTED_STATIC", no_argument),
        clib_create_argument('p', "port", "Set port", required_argument),
        clib_create_argument('r', "root", "Set root", required_argument)
    );

    struct option* opts = clib_get_options(args);

    char* usage = clib_format_text("%s [-h | -v] -e [-s | -S | -d] -p <port> -r <root>", argv[0]);
    int opt;
    while((opt = getopt_long(argc, argv, clib_generate_cli_format_string(args), opts, NULL)) != -1){
        switch (opt) {
            case 'h':
                clib_cli_help(args, usage, "Made by KDesp73");
                exit(0);
            case 'v':
                printf("webc v%s\n", VERSION);
                exit(0);
            case 'e':
                action.export = true;
                break;
            case 's':
                action.serve_static = true;
                break;
            case 'd':
                action.serve_dynamic = true;
                break;
            case 'S':
                action.serve_exported_static = true;
                break;
            case 'p':
                action.port = atoi(optarg);
                break;
            case 'r':
                action.root = optarg;
                break;
            default:
                exit(1);
        }

    }
    free(usage);
    return action;
}

void siginthandler(int params){
    INFO("Server closed");
    exit(0);
}

WEBCAPI void HandleAction(WebcAction action, Tree tree)
{
    if(action.export){
        ExportTree(tree);
    } 

    if(action.serve_static || action.serve_dynamic || action.serve_exported_static){
        if(action.port == 0){
            PANIC("port argument not set");
        }
        signal(SIGINT, siginthandler);
        INFO("Server started at port %d...", action.port);
        INFO("Press Ctrl+C to stop");
    }

    if(action.serve_static){
        ServeTree(action.port, tree);     
    } else if(action.serve_dynamic){
        PANIC("ACTION_SERVE_DYNAMIC is not implemented yet");
    } else if(action.serve_exported_static){
        if(action.root != NULL){
            ServeExportedRoot(action.port, action.root);
        } else {
            ServeExported(action.port, tree);
        }
    }
}

