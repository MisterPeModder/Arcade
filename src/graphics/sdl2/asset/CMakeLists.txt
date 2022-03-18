set(INCROOT ${PROJECT_SOURCE_DIR}/src/graphics/sdl2/asset)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/graphics/sdl2/asset)

set(SRC
    ${SRC}
    ${SRCROOT}/Texture.cpp
    ${INCROOT}/Texture.hpp
    PARENT_SCOPE
)

SET(INCDIRS
    ${INCDIRS}
    ${INCROOT}
    PARENT_SCOPE
)
