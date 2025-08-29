FROM ubuntu:24.04

# Install required dependencies for 32-bit compilation
RUN apt-get update && apt-get install -y \
    gcc-multilib \
    g++-multilib \
    cmake \
    ninja-build \
    git \
    curl \
    zip \
    unzip \
    tar \
    pkg-config \
    python3 \
    python3-setuptools \
    python3-pip \
    binutils \
    file \
    && dpkg --add-architecture i386 \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app