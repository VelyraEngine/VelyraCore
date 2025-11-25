# VelyraCore
## Build Status:
- ** Ubuntu (GCC)**: ![Ubuntu Build](https://github.com/VelyraEngine/VelyraCore/actions/workflows/main.yml/badge.svg?branch=main&job=build-ubuntu)
- ** Windows (MSVC)**: ![Windows Build](https://github.com/VelyraEngine/VelyraCore/actions/workflows/main.yml/badge.svg?branch=main&job=build-windows)

Core Components of the VelyraProject, like a windowing system and Context Manager

## Installation
### Ubuntu 20.04
Install the necessary packages
```shell
sudo apt install gcc g++ cmake git
```
To enjoy faster builds, consider installing the following packages as well.
```shell
sudo apt install ninja-build ccache mold
```

### Windows 11
Install MSVC using the installer found on the microsoft webpage.

## Build
The project is build with CMake, optional flags:

- `BUILD_TESTING`: When set to =ON, unit tests are build.
- `VELYRA_COMPILE_STRICT`: When set to =ON, enables strict compilation flags, such as `-Wall -Wextra -Werror` on GCC/Clang and `/W4 /WX` on MSVC.
- `VELYRA_COMPILE_RELAXED`: When set to =ON, build the project with no extra compilation flags. This results in faster compilation times, but less strict error checking.