#!/bin/bash

# Build script for xash3d-nodemod Docker image

set -e

IMAGE_NAME="xash3d-nodemod"
IMAGE_TAG="latest"

# Get current user information
USER_ID=$(id -u)
GROUP_ID=$(id -g)

echo "Building Docker image: ${IMAGE_NAME}:${IMAGE_TAG}"
echo "Using USER_ID=${USER_ID}, GROUP_ID=${GROUP_ID}"

docker build \
    --build-arg USER_ID=${USER_ID} \
    --build-arg GROUP_ID=${GROUP_ID} \
    --build-arg USERNAME=${USERNAME} \
    -t "${IMAGE_NAME}:${IMAGE_TAG}" .

echo "Docker image built successfully: ${IMAGE_NAME}:${IMAGE_TAG}"
echo "Built shared library should be available in the container at: /app/build/"