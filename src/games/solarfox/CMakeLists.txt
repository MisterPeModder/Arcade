set(INCROOT ${PROJECT_SOURCE_DIR}/src/games/solarfox)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/games/solarfox)

set(SRC
    ${SRCROOT}/entrypoint.cpp
)

set(INCDIRS ${INCROOT})

add_arcade_game(
    NAME solarfox
    INCLUDE_DIRS ${SRCROOT} ${INCROOT}
    SOURCES ${SRC}
)
