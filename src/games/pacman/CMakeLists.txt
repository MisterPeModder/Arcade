set(INCROOT ${PROJECT_SOURCE_DIR}/src/games/pacman)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/games/pacman)

set(SRC
    ${SRCROOT}/entrypoint.cpp
)

set(INCDIRS ${INCROOT})

add_arcade_game(
    NAME pacman
    INCLUDE_DIRS ${SRCROOT} ${INCROOT}
    SOURCES ${SRC}
)
