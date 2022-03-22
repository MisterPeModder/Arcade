set(INCROOT ${PROJECT_SOURCE_DIR}/src/graphics/common/include)

set(SRC
    ${INCROOT}/arcade/graphics/units.hpp
)

add_library(arcade_graphics_common INTERFACE ${SRC})

target_include_directories(arcade_graphics_common INTERFACE ${INCROOT})

