#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

int main(int argc, char *argv[]) 
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_tabs>\n", argv[0]);
        return 1;
    }

    int gnb = atoi(argv[1]);
    
    FILE* fp = fopen("r_time.txt", "r");
    FILE* fp2 = fopen("time.txt", "w");
    
    int i;
    char command[1024];
    
    int s[gnb];
    int times[gnb];
    
    // Read values from time.txt into the times array
    for (i = 0; i < gnb; i++) {
        if (fscanf(fp, "%d", &times[i]) != 1) {
            perror("Error reading value from time.txt");
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    
    int sum = 0;

    for (i = 1; i <= gnb; i++) 
    {
        s[i-1] = sum;
        sum += times[i-1];
        fprintf(fp2,"%d\n",sum);        

        snprintf(command, sizeof(command), 
                 "gnome-terminal --tab -- bash -c 'sudo /usr/local/bin/kubectl -n ran-simulator%d exec deploy/sim5g-simulator -- /root/go/src/my5G-RANTester/cmd/app ue; exec bash'", 
                 i);
        int result = system(command);
        
        if (result == -1) {
            perror("Error executing command");
            return 1;
        }
        
        sleep(times[i-1]);
        
    }
    fclose(fp2);

    return 0;
}

