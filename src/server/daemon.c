#include "extern/httpd.h"
#include "webc-server.h"
#include <ctype.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void start(WEBCServe serve, Cstr ip, int port, Cstr root, Tree tree);
void stop(void);
void restart(WEBCServe serve, Cstr ip, int port, Cstr root, Tree tree);

int valid_pid(char* pid)
{
    return (pid != NULL && strcmp(pid, "0") != 0 && strcmp(pid, "0\n") != 0);
}

void strip_whitespace(char *str) 
{
    char *end;

    while (isspace((unsigned char)*str)) {
        str++;
    }

    if (*str == 0) {
        return;
    }

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }

    *(end + 1) = '\0';
}

void start(WEBCServe serve, Cstr ip, int port, Cstr root, Tree tree) {
    if (clib_file_exists(PID_PATH)) {
        char* old_pid = clib_read_file(PID_PATH, "r");
        if (old_pid != NULL) {
            strip_whitespace(old_pid);
            if (valid_pid(old_pid)) {
                free(old_pid);
                WARN("WEBC server daemon is active. Restarting...");
                restart(serve, ip, port, root, tree);
                return;
            }
            free(old_pid);
        }
    }

    INFO("Daemon running...");
    daemonize();

    // Write the PID file
    pid_t pid = getpid();
    FILE *pid_file = fopen(PID_PATH, "w");
    if (pid_file == NULL) {
        perror("fopen failed");
        exit(EXIT_FAILURE);
    }
    fprintf(pid_file, "%d\n", pid);
    fclose(pid_file);

    // Start the server
    if (root == NULL) {
        serve.serve_tree(ip, port, tree);
    } else {
        serve.serve_root(ip, port, root);
    }

    _exit(EXIT_SUCCESS);
}


void stop(){
    char* pid_str = clib_read_file(PID_PATH, "r");
    if(pid_str != NULL) strip_whitespace(pid_str);
    if(!valid_pid(pid_str)){
        ERRO("Httpd server daemon is not active");
        return;
    }

    INFO("Stopping daemon...");
    kill(atoi(pid_str), SIGTERM);    

    char* command = clib_format_text("rm %s", PID_PATH);
    clib_execute_command(command);

    free(command);
    free(pid_str);
}

void restart(WEBCServe serve, Cstr ip, int port, Cstr root, Tree tree){
    stop();
    start(serve, ip, port, root, tree);
}

WEBCAPI void WEBC_Daemon(DaemonAction action, WEBCServe serve, Cstr ip, int port, Cstr root, Tree tree)
{
    switch(action){
        case DAEMON_START: 
            start(serve, ip, port, root, tree);
            break;
        case DAEMON_STOP:
            stop();
            break;
        case DAEMON_RESTART:
            restart(serve, ip, port, root, tree);
            break;
    }
}
