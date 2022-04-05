find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

set(INCROOT ${PROJECT_SOURCE_DIR}/src/graphics/sdl2)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/graphics/sdl2)

set(SRC
    ${SRCROOT}/AssetManager.cpp
    ${INCROOT}/AssetManager.hpp
    ${SRCROOT}/entrypoint.cpp
    ${SRCROOT}/event.cpp
    ${INCROOT}/event.hpp
    ${SRCROOT}/Renderer.cpp
    ${INCROOT}/Renderer.hpp
    ${SRCROOT}/Sdl2Display.cpp
    ${INCROOT}/Sdl2Display.hpp
)

set(INCDIRS ${INCROOT})

add_subdirectory(asset)
add_subdirectory(object)

add_arcade_graphics(
    NAME sdl2
    INCLUDE_DIRS ${SRCROOT} ${INCROOT}
    SOURCES ${SRC}
    DEPENDS_ON SDL2::Main SDL2::Image SDL2::TTF
)
