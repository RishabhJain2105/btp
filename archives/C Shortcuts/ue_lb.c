#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void execute_command(char *cmd, char *args[]) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        if (execvp(cmd, args) == -1) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process
        wait(NULL);
    }
}

int main()
{
    char *cmd13[] = {"sudo", "helm", "-n", "loadbalancer", "upgrade", "--install", "lb5g", "loxilb-helm-chart/", NULL};
    execute_command("sudo", cmd13);

    char *cmd14[] = {"sudo", "helm", "-n", "ran-simulator", "upgrade", "--install", "sim5g", "my5GRanTester-helm-chart/", NULL};
    execute_command("sudo", cmd14);

    char *cmd16[] = {"sudo", "kubectl", "-n", "loadbalancer", "get", "po", NULL};
    execute_command("sudo", cmd16);

    char *cmd17[] = {"sudo", "kubectl", "-n", "ran-simulator", "get", "po", NULL};
    execute_command("sudo", cmd17);
}    
