#!/bin/bash

IMAGE_NAME="vrajpat3ll/load_balancer"

echo "Select load balancer mode to tag and push:"
echo "1) consistent_hash"
echo "2) round_robin"
read -p "Enter choice [1 or 2]: " choice

case $choice in
  1)
    TAG="consistent_hash"
    ;;
  2)
    TAG="round_robin"
    ;;
  *)
    echo "Invalid choice. Exiting."
    exit 1
    ;;
esac

# Tag and push
sudo docker login
sudo docker build -t $IMAGE_NAME .

sudo docker tag $IMAGE_NAME $IMAGE_NAME:$TAG
echo "Tagged image as $IMAGE_NAME:$TAG"

sudo docker push $IMAGE_NAME:$TAG
echo "Pushed image $IMAGE_NAME:$TAG to Docker Hub"
