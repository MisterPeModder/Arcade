find_package(SDL2 REQUIRED)

set(INCROOT ${PROJECT_SOURCE_DIR}/src/graphics/sdl2)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/graphics/sdl2)

set(SRC
    ${SRCROOT}/entrypoint.cpp
)

set(INCDIRS ${INCROOT})

add_arcade_graphics(
    NAME sdl2
    INCLUDE_DIRS ${SRCROOT} ${INCROOT}
    SOURCES ${SRC}
    DEPENDS_ON SDL2::Main
)
