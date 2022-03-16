set(INCROOT ${PROJECT_SOURCE_DIR}/src/core/util)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/core/util)

set(SRC
    ${SRC}
    ${SRCROOT}/DynamicLibrary.cpp
    ${INCROOT}/DynamicLibrary.hpp
    PARENT_SCOPE
)

SET(INCDIRS
    ${INCDIRS}
    ${INCROOT}
    PARENT_SCOPE
)
