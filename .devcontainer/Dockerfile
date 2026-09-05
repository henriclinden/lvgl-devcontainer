FROM mcr.microsoft.com/devcontainers/base:debian-12

# Prevent interactive prompts during installation
ENV DEBIAN_FRONTEND=noninteractive

# Install C/C++ build tools, CMake, Ninja, and Wayland dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    ninja-build \
    gdb \
    pkg-config \
    git \
    libsdl2-dev \
    libdecor-0-dev \
    libdecor-0-plugin-1-cairo \
    libwayland-dev \
    libxkbcommon-dev \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Ensure groups exist inside the container before adding the user to them
RUN for group in video input render; do \
        getent group "$group" >/dev/null || groupadd -r "$group"; \
    done && \
    usermod -aG video,input,render vscode
