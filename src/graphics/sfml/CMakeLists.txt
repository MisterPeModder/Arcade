set(INCROOT ${PROJECT_SOURCE_DIR}/src/graphics/sfml)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/graphics/sfml)

set(SRC
    ${SRCROOT}/entrypoint.cpp
)

set(INCDIRS ${INCROOT})

add_arcade_graphics(
    NAME sfml
    INCLUDE_DIRS ${SRCROOT} ${INCROOT}
    SOURCES ${SRC}
)
