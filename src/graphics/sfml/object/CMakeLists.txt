set(INCROOT ${PROJECT_SOURCE_DIR}/src/graphics/sfml/object)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/graphics/sfml/object)

set(SRC
    ${SRC}
    ${SRCROOT}/Rectangle.cpp
    ${INCROOT}/Rectangle.hpp
    ${SRCROOT}/Text.cpp
    ${INCROOT}/Text.hpp
    PARENT_SCOPE
)

SET(INCDIRS
    ${INCDIRS}
    ${INCROOT}
    PARENT_SCOPE
)
