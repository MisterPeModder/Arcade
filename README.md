Arcade Interface
[![Build](https://img.shields.io/github/workflow/status/MisterPeModder/Arcade-Interface/CI)](https://github.com/MisterPeModder/Arcade-Interface/actions/workflows/ci.yml)
[![License](https://img.shields.io/github/license/MisterPeModder/Arcade-Interface)](https://github.com/MisterPeModder/Arcade-Interface)
====

# Building

## Command Line (via CMake)

Required tools:
- CMake 3.17 (minimum)

on Linux:
```sh
# Create the build directory
mkdir build && cd build 

# Configure the project
cmake .. -G 'Unix Makefiles' -DCMAKE_BUILD_TYPE=Release

# Build the executable and libraries
cmake --build .

# Return to previous directory
cd -
```

# Documentation

The documatation is available [online](https://misterpemodder.github.io/Arcade/).

## Local Docs

Required tools:
- Doxygen

on Linux:
```sh
# Run at the root of the project
doxygen

# Open the genrated pages
xdg-open doc/generated/html/index.html
```
