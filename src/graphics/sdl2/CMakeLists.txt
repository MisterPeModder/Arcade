find_package(SDL2 REQUIRED)

set(INCROOT ${PROJECT_SOURCE_DIR}/src/graphics/sdl2)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/graphics/sdl2)

set(SRC
    ${SRCROOT}/entrypoint.cpp
    ${SRCROOT}/event.cpp
    ${INCROOT}/event.hpp
    ${SRCROOT}/Sdl2Display.cpp
    ${INCROOT}/Sdl2Display.hpp
)

set(INCDIRS ${INCROOT})

add_arcade_graphics(
    NAME sdl2
    INCLUDE_DIRS ${SRCROOT} ${INCROOT}
    SOURCES ${SRC}
    DEPENDS_ON SDL2::Main
)
