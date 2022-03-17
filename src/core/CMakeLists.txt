set(INCROOT ${PROJECT_SOURCE_DIR}/src/core)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/core)

set(SRC
    ${SRCROOT}/arcade.cpp
    ${INCROOT}/arcade.hpp
    ${SRCROOT}/Core.cpp
    ${INCROOT}/Core.hpp
)

set(INCDIRS ${INCROOT})

add_subdirectory(util)

source_group("" FILES ${SRC})

set(LIB_NAME libarcade_core)
set(TARGET_NAME arcade)

add_library(libarcade_core ${SRC})

target_include_directories(${LIB_NAME} PRIVATE ${INCDIRS})
target_include_directories(${LIB_NAME} PUBLIC ${INCROOT})

target_link_libraries(libarcade_core PRIVATE arcade_interface dl)

add_executable(${TARGET_NAME} ${SRCROOT}/main.cpp)
target_link_libraries(${TARGET_NAME} PRIVATE ${LIB_NAME})

add_custom_command(TARGET ${TARGET_NAME} 
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${TARGET_NAME}> ${PROJECT_SOURCE_DIR})
