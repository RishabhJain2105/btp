#include <stdio.h>
#include <stdlib.h>

void writeToFile(int numRanTesters) {
    FILE *file = fopen("Start.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    fprintf(file, "cd 5gcore-sctp-loadbalancer\n\n");

    fprintf(file, "sudo kind create cluster --config config-3node.yml\n\n");
    fprintf(file, "sudo kubectl create -f multus-daemonset.yml\n\n");
    fprintf(file, "sudo kubectl create ns open5gs\n\n");
    fprintf(file, "sudo kubectl create ns loadbalancer\n\n");

    for (int i = 1; i <= numRanTesters; i++) {
        fprintf(file, "sudo kubectl create ns ran-simulator%d\n\n", i);
    }

    fprintf(file, "sudo curl -LO https://github.com/redhat-nfvpe/koko/releases/download/v0.82/koko_0.82_linux_amd64\n\n");
    fprintf(file, "sudo chmod +x koko_0.82_linux_amd64\n\n");
    fprintf(file, "sudo ./koko_0.82_linux_amd64 -d kind-worker,eth1 -d kind-worker2,eth1q\n\n");
    fprintf(file, "sudo modprobe sctp\n\n");
    fprintf(file, "sudo kubectl create -f cni-install.yml\n\n");
    fprintf(file, "sudo kubectl create -f core-5g-macvlan.yml\n\n");
    fprintf(file, "sudo helm -n open5gs upgrade --install core5g open5gs-helm-charts/\n\n");
    fprintf(file, "sudo kubectl -n open5gs get po\n\n");
    fprintf(file, "sudo kubectl apply -f service-account.yaml\n\n");
    fprintf(file, "sudo kubectl apply -f cluster-role.yaml\n\n");
    fprintf(file, "sudo kubectl apply -f cluster-role-binding.yaml\n\n");
    fprintf(file, "sudo helm -n loadbalancer upgrade --install lb Loadbalancer-helm-chart/\n\n");
    fprintf(file, "sudo kubectl -n loadbalancer get po\n\n");

    for (int i = 1; i <= numRanTesters; i++) {
        fprintf(file, "sudo helm -n ran-simulator%d upgrade --install sim5g my5GRanTester%d-helm-chart/\n\n", i, i);
    }
    
    for (int i = 1; i <= numRanTesters; i++) {
        fprintf(file, "sudo kubectl -n ran-simulator%d get po\n\n", i);
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_ran_testers>\n", argv[0]);
        return 1;
    }

    int numRanTesters = atoi(argv[1]);
    if (numRanTesters <= 0) {
        fprintf(stderr, "Number of RAN testers must be a positive integer.\n");
        return 1;
    }

    writeToFile(numRanTesters);

    return 0;
}

