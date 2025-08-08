#!/bin/bash

cd 5gcore-sctp-loadbalancer || exit

sudo kind create cluster --config config-3node.yml
sudo kubectl create -f multus-daemonset.yml

for ns in open5gs loadbalancer; do
  sudo kubectl create ns "$ns"
done

for i in {01..90}; do
  sudo kubectl create ns "ran-simulator$(printf "%02d" "$i")"
done

sudo curl -LO https://github.com/redhat-nfvpe/koko/releases/download/v0.82/koko_0.82_linux_amd64
sudo chmod +x koko_0.82_linux_amd64
sudo ./koko_0.82_linux_amd64 -d kind-worker,eth1 -d kind-worker2,eth1q

sudo modprobe sctp
sudo kubectl create -f cni-install.yml
sudo kubectl create -f core-5g-macvlan.yml
sudo helm -n open5gs upgrade --install core5g open5gs-helm-charts/
sudo kubectl -n open5gs get po

sudo kubectl apply -f service-account.yaml
sudo kubectl apply -f cluster-role.yaml
sudo kubectl apply -f cluster-role-binding.yaml

sudo helm -n loadbalancer upgrade --install lb Loadbalancer-helm-chart/
sudo kubectl -n loadbalancer get po

for i in {01..90}; do
  ns="ran-simulator$(printf "%02d" "$i")"
  chart="my5GRanTester${i}-helm-chart/"
  sudo helm -n "$ns" upgrade --install sim5g "$chart"
done

./monitor_pods.sh
