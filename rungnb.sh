#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <number_of_gnbs>"
    exit 1
fi

gnb_count=$1

if [ ! -f r_time.txt ]; then
    echo "Error: r_time.txt not found."
    exit 1
fi

readarray -t times < r_time.txt

if [ "${#times[@]}" -lt "$gnb_count" ]; then
    echo "Error: Not enough time entries in r_time.txt."
    exit 1
fi

for ((i=1; i<=gnb_count; i++)); do
    delay=${times[i-1]}
    
    gnome-terminal --tab -- bash -c "
        sudo /usr/local/bin/kubectl -n ran-simulator$i exec deploy/sim5g-simulator -- \
            bash -c 'cd /root/go/src/my5G-RANTester/cmd/app && ./app ue';
        exec bash"
    
    sleep "$delay"
done
