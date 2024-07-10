#include "extern/httpd.h"
#include "webc-actions.h"
#include "webc-server.h"
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


WEBCAPI WebcAction WEBC_ParseCliArgs(int argc, char** argv)
{
    if(argc == 1){
        WARN("No argument provided.");
        INFO("See %s --help", argv[0]);
        exit(1);
    }

    WebcAction action = {0};
    action.daemon_action = -1;

    CliArguments args = clib_make_cli_arguments(10,
        clib_create_argument('h', "help", "Prints this message", no_argument),
        clib_create_argument('v', "version", "Prints the version of the library", no_argument),
        clib_create_argument('e', "export", "Set action as EXPORT", no_argument),
        clib_create_argument('s', "serve-static", "Set action as SERVE_STATIC", no_argument),
        clib_create_argument('d', "serve-dynamic", "Set action as SERVE_DYNAMIC", no_argument),
        clib_create_argument('S', "serve-exported-static", "Set action as SERVE_EXPORTED_STATIC", no_argument),
        clib_create_argument('p', "port", "Set port", required_argument),
        clib_create_argument('r', "root", "Set root", required_argument),
        clib_create_argument('i', "ip", "Set ip address", required_argument),
        clib_create_argument('D', "daemon", "Handle daemon", required_argument)
    );

    struct option* opts = clib_get_options(args);

    char* usage = clib_format_text("%s [-h | -v] -e [-s | -S | -d -D [START | STOP | RESTART]] -p <port> -r <root> -i <ip>", argv[0]);
    char* format = clib_generate_cli_format_string(args);
    int opt;
    while((opt = getopt_long(argc, argv, format, opts, NULL)) != -1){
        switch (opt) {
            case 'h':
                clib_cli_help(args, usage, "Made by KDesp73");
                clib_clean_arguments(&args);
                exit(0);
            case 'v':
                printf("webc v%s\n", VERSION);
                clib_clean_arguments(&args);
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
            case 'i':
                action.ip = optarg;
                break;
            case 'D':
                if(strcmp(optarg, "START") == 0){
                    action.daemon_action = DAEMON_START;
                } else if(strcmp(optarg, "STOP") == 0){
                    action.daemon_action = DAEMON_STOP;
                } else if(strcmp(optarg, "RESTART") == 0){
                    action.daemon_action = DAEMON_RESTART;
                } else {
                    PANIC("Invalid daemon action: %s", optarg);
                }
                break;
            default:
                exit(1);
        }
    }

    free(usage);
    free(format);
    free(opts);
    clib_clean_arguments(&args);
    return action;
}

void siginthandler(int params){
    printf("\n");
    INFO("Server closed");
    exit(0);
}

WEBCAPI void WEBC_HandleAction(WebcAction action, Tree tree)
{
    char* ip = action.ip;
    if(ip == NULL){
        ip = "127.0.0.1";
    }

    if(action.export){
        WEBC_ExportTree(tree);
    } 

    if(action.daemon_action == DAEMON_STOP){
        WEBC_Daemon(DAEMON_STOP, (WEBCServe) WEBC_ServeExportedRoot, NULL, 0, NULL, tree);
        return;
    }

    if(
        action.serve_static || 
        action.serve_dynamic || 
        action.serve_exported_static || 
        action.daemon_action == DAEMON_START || 
        action.daemon_action == DAEMON_RESTART
    ){
        if(action.port == 0){
            PANIC("port argument not set");
        }
        signal(SIGINT, siginthandler);

        INFO("Server started at %s:%d...", ip, action.port);

        if(action.daemon_action == -1)
            INFO("Press Ctrl+C to stop");
        else 
            INFO("Run `./exe -D STOP` to stop the daemon");
    }

    if(action.serve_static){
        if(action.daemon_action == -1)
            WEBC_ServeTree(ip, action.port, tree);     
        else 
            WEBC_Daemon(action.daemon_action, (WEBCServe) WEBC_ServeTree, ip, action.port, NULL, tree);
    } else if(action.serve_dynamic){
        PANIC("ACTION_SERVE_DYNAMIC is not implemented yet");
    } else if(action.serve_exported_static){
        if(action.root != NULL){
            if(action.daemon_action == -1)
                WEBC_ServeExportedRoot(ip, action.port, action.root);
            else
                WEBC_Daemon(action.daemon_action, (WEBCServe) WEBC_ServeExportedRoot, ip, action.port, action.root, tree);
        } else {
            if(action.daemon_action == -1)
                WEBC_ServeExported(ip, action.port, tree);
            else
                WEBC_Daemon(action.daemon_action, (WEBCServe) WEBC_ServeExported, ip, action.port, action.root, tree);
        }
    }

    if(action.root != NULL)
        free(action.root);

    WEBC_CleanTree(tree);
}

