set(INCROOT ${PROJECT_SOURCE_DIR}/src/games/qix)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/games/qix)

set(SRC
    ${SRCROOT}/entrypoint.cpp
)

set(INCDIRS ${INCROOT})

add_arcade_game(
    NAME qix
    INCLUDE_DIRS ${SRCROOT} ${INCROOT}
    SOURCES ${SRC}
)
