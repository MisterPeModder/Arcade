set(INCROOT ${PROJECT_SOURCE_DIR}/include)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src)

set(SRC
    ${INCROOT}/arcade/Event.hpp
    ${INCROOT}/arcade/IAsset.hpp
    ${INCROOT}/arcade/IDisplay.hpp
    ${INCROOT}/arcade/IGame.hpp
    ${INCROOT}/arcade/IGameObject.hpp
    ${INCROOT}/arcade/types.hpp
    ${SRCROOT}/placeholder.cpp
)

set(INCDIRS ${INCROOT})

source_group("" FILES ${SRC})

add_library(libarcade_interface ${SRC})

target_include_directories(libarcade_interface PRIVATE ${INCDIRS} ${SRCROOT})
target_include_directories(libarcade_interface PUBLIC ${INCROOT})
