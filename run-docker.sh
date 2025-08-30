#!/bin/bash

# Run script for xash3d-nodemod Docker container

set -e

IMAGE_NAME="xash3d-nodemod"
IMAGE_TAG="latest"

# Get current user information  
USERNAME=${USER:-nodemod}

echo "Running Docker container: ${IMAGE_NAME}:${IMAGE_TAG}"
echo "Running as user: ${USERNAME}"

# Mount the current directory to /app in the container
# This allows the build outputs to be accessible on the host
docker run -it --rm \
    -v "$(pwd):/app" \
    -w /app \
    "${IMAGE_NAME}:${IMAGE_TAG}" \
    bash

echo "Docker container session ended"