set(INCROOT ${PROJECT_SOURCE_DIR}/src/graphics/ncurses)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/graphics/ncurses)

set(SRC
    ${SRCROOT}/entrypoint.cpp
)

set(INCDIRS ${INCROOT})

add_arcade_graphics(
    NAME ncurses
    INCLUDE_DIRS ${SRCROOT} ${INCROOT}
    SOURCES ${SRC}
)
