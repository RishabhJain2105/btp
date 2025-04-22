#!/bin/bash

# Hide cursor for cleaner look
tput civis

# Restore cursor on exit
trap "tput cnorm; exit" INT

while true; do
    pods_output=$(sudo kubectl get pods --all-namespaces -o wide)
    clear

    echo -e "$pods_output"

    Completed=$(echo "$pods_output" | grep Completed | wc -l)
    Unknown=$(echo "$pods_output" | grep Unknown | wc -l)
    Running=$(echo "$pods_output" | grep Running | wc -l)
    Pending=$(echo "$pods_output" | grep Pending | wc -l)
    Terminating=$(echo "$pods_output" | grep Terminating | wc -l)
    ContainerCreating=$(echo "$pods_output" | grep ContainerCreating | wc -l)

    echo -e ""
    echo "🔁 Pod Status Counts:"
    echo "          Unknown: $Unknown"
    echo "          Running: $Running"
    echo "          Pending: $Pending"
    echo "      Terminating: $Terminating"
    echo "        Completed: $Completed"
    echo "ContainerCreating: $ContainerCreating"

    sleep 1
done
