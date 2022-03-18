set(INCROOT ${PROJECT_SOURCE_DIR}/src/graphics/sdl2/object)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/graphics/sdl2/object)

set(SRC
    ${SRC}
    ${SRCROOT}/Rectangle.cpp
    ${INCROOT}/Rectangle.hpp
    PARENT_SCOPE
)

SET(INCDIRS
    ${INCDIRS}
    ${INCROOT}
    PARENT_SCOPE
)
