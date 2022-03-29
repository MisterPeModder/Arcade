find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

set(INCROOT ${PROJECT_SOURCE_DIR}/src/graphics/sfml)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/graphics/sfml)

set(SRC
    ${SRCROOT}/entrypoint.cpp
    ${SRCROOT}/event.cpp
    ${INCROOT}/event.hpp
    ${SRCROOT}/SfmlDisplay.cpp
    ${INCROOT}/SfmlDisplay.hpp
)

set(INCDIRS ${INCROOT})

add_arcade_graphics(
    NAME sfml
    INCLUDE_DIRS ${SRCROOT} ${INCROOT}
    SOURCES ${SRC}
    DEPENDS_ON ${SFML_LIBRARIES}
)
