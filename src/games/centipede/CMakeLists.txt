set(INCROOT ${PROJECT_SOURCE_DIR}/src/games/centipede)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/games/centipede)

set(SRC
    ${SRCROOT}/entrypoint.cpp
    ${SRCROOT}/ExampleGame.cpp
)

set(INCDIRS ${INCROOT})

add_arcade_game(
    NAME centipede
    INCLUDE_DIRS ${SRCROOT} ${INCROOT}
    SOURCES ${SRC}
)
