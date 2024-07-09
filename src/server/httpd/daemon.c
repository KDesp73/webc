#include "extern/httpd.h"
#include <sys/types.h>

void start_daemon(server_t server);
void stop_daemon(void);
void restart_daemon(server_t server);

void start_daemon(server_t server){
    if(clib_file_exists(PID_PATH)){
        char* old_pid = clib_read_file(PID_PATH, "r");
        if(old_pid != NULL){
            free(old_pid);
            WARN("Httpd server daemon is active. Restarting...");
            restart_daemon(server);
        }
    }

    pid_t pid;

    pid = fork();

    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    if (setsid() < 0) {
        exit(EXIT_FAILURE);
    }

    chdir("/");

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    char* pid_str = clib_format_text("%d", pid);

    FILE *fptr;
    fptr = fopen(PID_PATH, "w");
    fprintf(fptr, "%s", pid_str);
    fclose(fptr); 
    free(pid_str);

    run_server(server);
}

void stop_daemon(){
    char* pid_str = clib_read_file(PID_PATH, "r");
    if(pid_str == NULL){
        PANIC("Httpd server daemon is not active");
    }

    INFO("Stopping daemon...");
    char* command = clib_format_text("kill %s", pid_str);
    clib_execute_command(command);
    free(command);
}

void restart_daemon(server_t server){
    stop_daemon();
    start_daemon(server);
}

HTTPDAPI int run_daemon(DaemonAction action, server_t server)
{

    if(action == DAEMON_START){
        start_daemon(server);
    } else if(action == DAEMON_STOP){
        stop_daemon();
    } else if(action == DAEMON_RESTART){
        restart_daemon(server);
    }

    return action;
}
