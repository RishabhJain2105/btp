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

int main(int argc, char* argv[]) 
{

    int i = atoi(argv[1]);
    
    i+=48;
    
    char namespace[] = "ran-simulator"; // Using a character array
	if (i > 48 && i < 58) // Ensure i is between '0' and '9'
    namespace[13] = (char)i;

    char *cmd0[] = {"sudo", "kubectl", "-n", namespace, "exec", "-ti", "deploy/sim5g-simulator", "bash", NULL};
    execute_command("sudo", cmd0);

}
