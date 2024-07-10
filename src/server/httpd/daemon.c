#include "extern/httpd.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void start_daemon(server_t server);
void stop_daemon(void);
void restart_daemon(server_t server);

HTTPDAPI void daemonize(void) 
{
    pid_t pid;

    // Fork the first time
    pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        // Parent process
        exit(EXIT_SUCCESS);
    }

    // Create a new session
    if (setsid() < 0) {
        perror("setsid failed");
        exit(EXIT_FAILURE);
    }

    // Fork the second time
    pid = fork();
    if (pid < 0) {
        perror("second fork failed");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        // First child process
        exit(EXIT_SUCCESS);
    }

    // Set the file mode creation mask to 0
    umask(0);

    // Close standard file descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Redirect standard file descriptors to /dev/null
    int fd = open("/dev/null", O_RDWR);
    if (fd != STDIN_FILENO) {
        perror("Failed to reopen stdin to /dev/null");
        exit(EXIT_FAILURE);
    }
    if (dup2(fd, STDOUT_FILENO) != STDOUT_FILENO) {
        perror("Failed to reopen stdout to /dev/null");
        exit(EXIT_FAILURE);
    }
    if (dup2(fd, STDERR_FILENO) != STDERR_FILENO) {
        perror("Failed to reopen stderr to /dev/null");
        exit(EXIT_FAILURE);
    }
    if (fd > STDERR_FILENO) {
        if (close(fd) < 0) {
            perror("Failed to close extra file descriptor");
            exit(EXIT_FAILURE);
        }
    }
}

void start_daemon(server_t server){
    if(clib_file_exists(PID_PATH)){
        char* old_pid = clib_read_file(PID_PATH, "r");
        if(old_pid != NULL){
            free(old_pid);
            WARN("Httpd server daemon is active. Restarting...");
            restart_daemon(server);
        }
        free(old_pid);
    }

    daemonize();

    pid_t pid = getpid();
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
