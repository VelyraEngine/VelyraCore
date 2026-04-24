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
- `BUILD_SANDBOX`: When set to =ON, the sandbox application is build, allowing to test certain features.
- `VL_COMPILE_STRICT`: When set to =ON, enables strict compilation flags, such as `-Wall -Wextra -Werror` on GCC/Clang and `/W4 /WX` on MSVC.
- `ENABLE_COVERAGE`: When set to =ON, enables code coverage instrumentation for measuring test coverage metrics.

## Testing and Coverage

VelyraCore includes a comprehensive test suite using Google Test. To build and run tests:

```shell
cmake -DBUILD_TESTING=ON ..
cmake --build .
ctest
```

### Code Coverage

To generate code coverage reports (requires `lcov` and `gcovr`):

```shell
# Install coverage tools (Ubuntu)
sudo apt install lcov gcovr

# Configure with coverage enabled
cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON -DBUILD_TESTING=ON ..
cmake --build .

# Generate HTML coverage report
cmake --build . --target coverage_gcovr_html_TestVelyraCore

# Or get a quick summary
cmake --build . --target coverage_summary_TestVelyraCore
```

For detailed coverage instructions, see [COVERAGE.md](COVERAGE.md).
- `VL_COMPILE_RELAXED`: When set to =ON, build the project with no extra compilation flags. This results in faster compilation times, but less strict error checking.