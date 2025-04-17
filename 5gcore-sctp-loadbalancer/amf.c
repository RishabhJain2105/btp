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

int main(int argc,char* argv[]) 
{
    int n;
    n=atoi(argv[1]);
    if(n==3)
    {
	char *cmd0[] = {"sudo", "kubectl", "-n", "open5gs", "logs", "deploy/core5g-amf-3-deployment", NULL};
	execute_command("sudo", cmd0);
    }    
    if(n==2)
    {
	char *cmd0[] = {"sudo", "kubectl", "-n", "open5gs", "logs", "deploy/core5g-amf-2-deployment", NULL};
	execute_command("sudo", cmd0);
    }
    else if(n==1)
    {
	char *cmd0[] = {"sudo", "kubectl", "-n", "open5gs", "logs", "deploy/core5g-amf-1-deployment", NULL};
	execute_command("sudo", cmd0);
    }    
}
