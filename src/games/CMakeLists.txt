# Adds a new Arcade game
function(add_arcade_game)
    set(oneValueArgs NAME)
    set(multiValueArgs INCLUDE_DIRS SOURCES)
    cmake_parse_arguments(add_arcade_game "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})


    # Arguments checking
    if (NOT DEFINED add_arcade_game_NAME)
        message(FATAL_ERROR "missing graphics backend name")
    endif()
    if (NOT DEFINED add_arcade_game_SOURCES)
        message(FATAL_ERROR "missing graphics backend sources")
    endif()

    set(SRC ${add_arcade_game_SOURCES})
    set(TARGET_NAME arcade_${add_arcade_game_NAME})
    set(INCDIRS ${add_arcade_game_INCLUDE_DIRS})

    source_group("" FILES ${SRC})

    # Declare the library
    add_library(${TARGET_NAME} SHARED ${SRC})

    # Remove the 'lib' prefix
    set_target_properties(${TARGET_NAME} PROPERTIES PREFIX "")

    # Add ${INCDIRS} and ${SRCROOT} to the lib's private include path
    target_include_directories(${TARGET_NAME} PRIVATE ${INCDIRS} ${SRCROOT})

    target_link_libraries(${TARGET_NAME} PRIVATE arcade_interface)

    # Copy the library to lib/${TARGET_NAME}
    add_custom_command(TARGET ${TARGET_NAME} 
    	POST_BUILD
    	COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/lib
    	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${TARGET_NAME}> ${PROJECT_SOURCE_DIR}/lib)
endfunction()

# Games
# add_subdirectory(nibbler)
