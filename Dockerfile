# Universal Dockerfile for xv6 - Works on Windows, Mac (Intel & Apple Silicon), and Ubuntu
FROM --platform=linux/amd64 ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    gdb \
    git \
    qemu-system-x86 \
    gcc \
    g++ \
    gcc-multilib \
    vim \
    nano \
    curl \
    wget \
    make \
    sudo \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /xv6

CMD ["/bin/bash"]