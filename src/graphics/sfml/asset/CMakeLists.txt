set(INCROOT ${PROJECT_SOURCE_DIR}/src/graphics/sfml/asset)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/graphics/sfml/asset)

set(SRC
    ${SRC}
    ${SRCROOT}/Font.cpp
    ${INCROOT}/Font.hpp
    ${SRCROOT}/Texture.cpp
    ${INCROOT}/Texture.hpp
    PARENT_SCOPE
)

SET(INCDIRS
    ${INCDIRS}
    ${INCROOT}
    PARENT_SCOPE
)
