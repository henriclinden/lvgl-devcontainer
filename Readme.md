# LVGL in Docker Container

This project demonstrates how to set up and develop LVGL (Light and Versatile Graphics Library) applications within a Docker container on Ubuntu, with full GUI support through X11/Wayland forwarding.

## Overview

LVGL is a free and open-source embedded graphics library that creates beautiful UIs for microcontrollers and embedded systems. This dev container provides a complete development environment for building and testing LVGL applications on Linux.

## Prerequisites

- **Ubuntu 24.04** (or compatible Linux distribution)
- **Docker** installed and running
- **VS Code** with Remote - Containers extension
- X11 or Wayland display server running

## Quick Start

### 0. Clone the Repository and init Submodules

Since this project uses LVGL as a Git submodule, you need to clone it.

```bash
git clone https://github.com/henriclinden/lvgl-devcontainer.git
cd lvgl-devcontainer
git clone --recursive -b release/v9.1 https://github.com/lvgl/lvgl.git
```

### 1. Allow X11/Wayland Access to Container

Before opening the container in VS Code, you need to grant the container access to your display server:

```bash
xhost +local:root
xhost +local:vscode
```

These commands allow both the root user and vscode user to connect to your X11/Wayland display. Run these in your host terminal before starting the dev container.

### 2. Update Docker Render Group ID

The `devcontainer.json` file includes `"--group-add=993"` which grants access to the render device. However, the render group ID may differ on your system.

**To find your render group ID:**

```bash
getent group render | cut -d: -f3
```

**Then update the line in `.devcontainer/devcontainer.json`:**

Replace:
```json
"--group-add=993"
```

With the actual group ID returned by the command above (e.g., `"--group-add=992"`).

### 3. Open in VS Code

1. Clone or open this repository in VS Code
2. When prompted, select "Reopen in Container"
3. Wait for the container to build and start
4. The LVGL development environment is ready!

## Building and Running

The project uses CMake for building:

```bash
cmake -B build -G Ninja
cmake --build build
./build/my_lvgl_app
```

Or use VS Code's CMake Tools extension for building and debugging with the GUI.

## Project Structure

- **`main.c`** - Entry point for the LVGL application
- **`lv_conf.h`** - LVGL configuration file
- **`CMakeLists.txt`** - CMake build configuration
- **`.devcontainer/`** - Docker container configuration
- **`lvgl/`** - LVGL library source code (submodule)
- **`build/`** - Build output directory

## Display Support

This container supports both:
- **X11** - Traditional X Window System
- **Wayland** - Modern display protocol

The container automatically falls back between them based on availability. GPU acceleration is enabled through:
- `/dev/dri` device access (GPU rendering)
- `render` group membership (hardware acceleration)
- IPC host mode (shared memory for display)

## Troubleshooting

### Application won't display
- Ensure you ran `xhost +local:root` and `xhost +local:vscode` before starting the container
- Check that your display server is running: `echo $DISPLAY` or `echo $WAYLAND_DISPLAY`
- Verify the render group ID is correct for your system

### Permission denied errors
- Run `getent group render | cut -d: -f3` to verify the correct group ID
- Update the `--group-add` value in `.devcontainer/devcontainer.json`
- Rebuild the container

## Resources

- [LVGL Official Documentation](https://docs.lvgl.io/)
- [LVGL GitHub Repository](https://github.com/lvgl/lvgl)
- [VS Code Remote - Containers](https://code.visualstudio.com/docs/remote/containers)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

Note: The LVGL library included as a submodule has its own licensing - see the `lvgl/` directory for more information. 