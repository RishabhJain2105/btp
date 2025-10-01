#include "utils.h"

#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>

void execute_command(const char *cmd, char *const args[]) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return;  // don't exit entire program from helper
    } else if (pid == 0) {
        // Child process
        if (execvp(cmd, args) == -1) {
            perror("execvp");
            _exit(EXIT_FAILURE);
        }
    } else {
        // Parent process: wait for child
        int status = 0;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
        }
    }
}