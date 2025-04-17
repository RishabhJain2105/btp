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

    char *cmd0[] = {"sudo", "kind", "delete", "cluster", NULL};
    execute_command("sudo", cmd0);
    
    char *cmd1[] = {"sudo", "kind", "create", "cluster", "--config", "config-3node.yml", NULL};
    execute_command("sudo", cmd1);

    char *cmd2[] = {"sudo", "kubectl", "create", "-f", "multus-daemonset.yml", NULL};
    execute_command("sudo", cmd2);

    char *cmd3[] = {"sudo", "curl", "-LO", "https://github.com/redhat-nfvpe/koko/releases/download/v0.82/koko_0.82_linux_amd64", NULL};
    execute_command("sudo", cmd3);

    char *cmd4[] = {"sudo", "chmod", "+x", "koko_0.82_linux_amd64", NULL};
    execute_command("sudo", cmd4);

    char *cmd5[] = {"sudo", "./koko_0.82_linux_amd64", "-d", "kind-worker,eth1", "-d", "kind-worker2,eth1", NULL};
    execute_command("sudo", cmd5);

    char *cmd6[] = {"sudo", "kubectl", "create", "ns", "open5gs", NULL};
    execute_command("sudo", cmd6);

    char *cmd7[] = {"sudo", "kubectl", "create", "ns", "loadbalancer", NULL};
    execute_command("sudo", cmd7);

    char *cmd8[] = {"sudo", "kubectl", "create", "ns", "ran-simulator", NULL};
    execute_command("sudo", cmd8);
    
    char *cmd81[] = {"sudo", "kubectl", "create", "ns", "ran-simulator2", NULL};
    execute_command("sudo", cmd81);
    
    char *cmd82[] = {"sudo", "kubectl", "create", "ns", "ran-simulator3", NULL};
    execute_command("sudo", cmd82);

    char *cmd9[] = {"sudo", "modprobe", "sctp", NULL};
    execute_command("sudo", cmd9);

    char *cmd10[] = {"sudo", "kubectl", "create", "-f", "cni-install.yml", NULL};
    execute_command("sudo", cmd10);

    char *cmd11[] = {"sudo", "kubectl", "create", "-f", "core-5g-macvlan.yml", NULL};
    execute_command("sudo", cmd11);

    char *cmd12[] = {"sudo", "helm", "-n", "open5gs", "upgrade", "--install", "core5g", "open5gs-helm-charts/", NULL};
    execute_command("sudo", cmd12);

    char *cmd13[] = {"sudo", "helm", "-n", "loadbalancer", "upgrade", "--install", "lb5g", "loxilb-helm-chart/", NULL};
    execute_command("sudo", cmd13);

    char *cmd14[] = {"sudo", "helm", "-n", "ran-simulator", "upgrade", "--install", "sim5g", "my5GRanTester-helm-chart/", NULL};
    execute_command("sudo", cmd14);
    
    char *cmd141[] = {"sudo", "helm", "-n", "ran-simulator2", "upgrade", "--install", "sim5g", "my5GRanTester2-helm-chart/", NULL};
    execute_command("sudo", cmd141);
    
    char *cmd142[] = {"sudo", "helm", "-n", "ran-simulator3", "upgrade", "--install", "sim5g", "my5GRanTester3-helm-chart/", NULL};
    execute_command("sudo", cmd142);

    return 0;
}


