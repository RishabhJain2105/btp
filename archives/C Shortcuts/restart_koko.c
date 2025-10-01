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
    char *cmd1[] = {"sudo", "systemctl", "stop", "docker", NULL};
    execute_command("sudo", cmd1);
    
    char *cmd2[] = {"sudo", "systemctl", "start", "docker", NULL};
    execute_command("sudo", cmd2);
    
    char *cmd3[] = {"sudo", "curl", "-LO", "https://github.com/redhat-nfvpe/koko/releases/download/v0.82/koko_0.82_linux_amd64", NULL};
    execute_command("sudo", cmd3);

    char *cmd4[] = {"sudo", "chmod", "+x", "koko_0.82_linux_amd64", NULL};
    execute_command("sudo", cmd4);

    char *cmd5[] = {"sudo", "./koko_0.82_linux_amd64", "-d", "kind-worker,eth1", "-d", "kind-worker2,eth1", NULL};
    execute_command("sudo", cmd5);
    
    /*char *cmd6[] = {"sudo", "helm", "-n", "ran-simulator2", "uninstall", "sim5g", NULL};
    execute_command("sudo", cmd6);
    
    char *cmd8[] = {"sudo", "helm", "-n", "ran-simulator2", "upgrade", "--install", "sim5g", "my5GRanTester2-helm-chart/", NULL};
    execute_command("sudo", cmd8);
    
    char *cmd7[] = {"sudo", "helm", "-n", "ran-simulator3", "uninstall", "sim5g", NULL};
    execute_command("sudo", cmd7);
    
    char *cmd9[] = {"sudo", "helm", "-n", "ran-simulator3", "upgrade", "--install", "sim5g", "my5GRanTester3-helm-chart/", NULL};
    execute_command("sudo", cmd9);*/
}    
