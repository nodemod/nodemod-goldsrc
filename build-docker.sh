#!/bin/bash

# Build script for xash3d-nodemod Docker image

set -e

IMAGE_NAME="xash3d-nodemod"
IMAGE_TAG="latest"

echo "Building Docker image: ${IMAGE_NAME}:${IMAGE_TAG}"

docker build -t "${IMAGE_NAME}:${IMAGE_TAG}" .

echo "Docker image built successfully: ${IMAGE_NAME}:${IMAGE_TAG}"
echo "Built shared library should be available in the container at: /app/build/"