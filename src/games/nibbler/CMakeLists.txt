set(INCROOT ${PROJECT_SOURCE_DIR}/src/games/nibbler)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/games/nibbler)

set(SRC
    ${SRCROOT}/entrypoint.cpp
    ${SRCROOT}/Nibbler.cpp
    ${SRCROOT}/Nibbler.hpp
)

set(INCDIRS ${INCROOT})

add_arcade_game(
    NAME nibbler
    INCLUDE_DIRS ${SRCROOT} ${INCROOT}
    SOURCES ${SRC}
)
