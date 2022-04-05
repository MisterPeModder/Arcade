find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

set(INCROOT ${PROJECT_SOURCE_DIR}/src/graphics/sfml)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/graphics/sfml)

set(SRC
    ${SRCROOT}/AssetManager.cpp
    ${INCROOT}/AssetManager.hpp
    ${SRCROOT}/entrypoint.cpp
    ${SRCROOT}/event.cpp
    ${INCROOT}/event.hpp
    ${SRCROOT}/Renderer.cpp
    ${INCROOT}/Renderer.hpp
    ${SRCROOT}/SfmlDisplay.cpp
    ${INCROOT}/SfmlDisplay.hpp
)

set(INCDIRS ${INCROOT})

add_subdirectory(asset)
add_subdirectory(object)

add_arcade_graphics(
    NAME sfml
    INCLUDE_DIRS ${SRCROOT} ${INCROOT}
    SOURCES ${SRC}
    DEPENDS_ON ${SFML_LIBRARIES}
)
