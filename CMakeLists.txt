cmake_minimum_required(VERSION 3.17.0)

project(Arcade LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Add ./cmake to the module path
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_compile_options(
    "-Wall" "-Wextra" "-Wshadow" "-pedantic" "-Wpointer-arith"
    "$<$<CONFIG:RELEASE>:-O3;-Werror>"
    "$<$<CONFIG:DEBUG>:-O0;-g3;-ggdb>"
)

add_subdirectory(src)

