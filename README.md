Arcade
====

# Building

## Command Line (via CMake)

Required tools:
- CMake 3.17 (minimum)

```sh
# Creating the build directory
mkdir build && cd build 

# Configuring the project
cmake .. -G 'Unix Makefiles' -DCMAKE_BUILD_TYPE=Release

# Building the executable and libraries
cmake --build .

# Returning to previous directory
cd -
```
