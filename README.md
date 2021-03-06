Arcade
[![Build](https://img.shields.io/github/workflow/status/MisterPeModder/Arcade/CI)](https://github.com/MisterPeModder/Arcade/actions/workflows/ci.yml)
[![License](https://img.shields.io/github/license/MisterPeModder/Arcade)](https://github.com/MisterPeModder/Arcade)
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

## Groups

| Group A                                        | Group B                                        |
|------------------------------------------------|------------------------------------------------|
| Andreas LEROUX <andreas.leroux@epitech.eu>     | Yanis GUAYE <yanis.guaye@epitech.eu>           |
| Nathan ROUSSEAU <nathan.rousseau@epitech.eu>   | Julien LAFARGUE <julien.lafargue@epitech.eu>   |
| Johan CHRILLESEN <johan.chrillesen@epitech.eu> | Quentin Leroueil <quentin.leroueil@epitech.eu> |
