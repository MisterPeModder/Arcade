set(INCROOT ${PROJECT_SOURCE_DIR}/include)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src)

set(SRC
    ${INCROOT}/arcade/interface.hpp
    ${SRCROOT}/placeholder.cpp
)

set(INCDIRS ${INCROOT})

source_group("" FILES ${SRC})

add_library(libarcade_interface ${SRC})

target_include_directories(libarcade_interface PRIVATE ${INCDIRS} ${SRCROOT})
target_include_directories(libarcade_interface PUBLIC ${INCROOT})
