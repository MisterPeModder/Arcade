set(SRCROOT ${PROJECT_SOURCE_DIR}/doc/examples)

set(SRC
    ${SRCROOT}/ExampleDisplay.cpp
    ${SRCROOT}/ExampleGame.cpp
)

add_library(arcade_examples ${SRC})

target_link_libraries(arcade_examples PRIVATE arcade_interface)
