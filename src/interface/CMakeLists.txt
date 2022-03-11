cmake_minimum_required(VERSION 3.17.0)

project(Arcade-Interface LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(INCROOT ${PROJECT_SOURCE_DIR}/include)

set(SRC
    ${INCROOT}/arcade/Event.hpp
    ${INCROOT}/arcade/IAsset.hpp
    ${INCROOT}/arcade/IDisplay.hpp
    ${INCROOT}/arcade/IGame.hpp
    ${INCROOT}/arcade/IGameObject.hpp
    ${INCROOT}/arcade/types.hpp
)

add_library(libarcade_interface INTERFACE ${SRC})

target_include_directories(libarcade_interface INTERFACE ${INCROOT})

