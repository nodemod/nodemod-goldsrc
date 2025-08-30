FROM ubuntu:24.04

# Accept user information as build arguments
ARG USER_ID=1000
ARG GROUP_ID=1000

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
    sudo \
    && dpkg --add-architecture i386 \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Create a user with matching UID/GID (rename existing if needed)
RUN groupadd -g ${GROUP_ID} nodemod 2>/dev/null || groupmod -n nodemod $(getent group ${GROUP_ID} | cut -d: -f1) && \
    useradd -u ${USER_ID} -g ${GROUP_ID} -m -s /bin/bash nodemod 2>/dev/null || usermod -l nodemod -d /home/nodemod -m $(getent passwd ${USER_ID} | cut -d: -f1) && \
    echo "nodemod ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

# Set working directory and change ownership
WORKDIR /app
RUN chown -R nodemod:nodemod /app

# Switch to the created user
USER nodemod