cmake_minimum_required (VERSION 3.10)

#
# Project
#

project (simple-gl-snippets VERSION 0.1 LANGUAGES CXX)

#
# Globals
#

set(CMAKE_VERBOSE_MAKEFILE 1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#
# Libraries
#

# glfw
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)                                                       
#find_package(glfw3 REQUIRED)

# opengl
cmake_policy(SET CMP0072 NEW)    
find_package(OpenGL REQUIRED)

# glew
find_package(GLEW REQUIRED)

# glm
find_package(glm REQUIRED)

set(EXTRA_LIBS -lm)

#
# Generate compile_commands.json for vim help
#

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

IF (EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
    EXECUTE_PROCESS( 
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
        ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
    )
ENDIF ()

#
# Targets
#

set(all_libs
    ${GLFW_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${GLM_LIBRARIES}
    ${EXTRA_LIBS}
)

set(all_srcs)



add_executable(one src/one.cc ${all_srcs})

set_property(TARGET one PROPERTY CXX_STANDARD 17)
set_property(TARGET one PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET one PROPERTY CXX_EXTENSIONS OFF)
set_property(TARGET one APPEND PROPERTY COMPILE_DEFINITIONS GLM_ENABLE_EXPERIMENTAL=1)
set_property(TARGET one APPEND PROPERTY LINK_LIBRARIES ${all_libs})


if (APPLE)
# nothing now
endif (APPLE)

if (MSVC)
# nothing now
endif (MSVC)

if (UNIX)
    set_property(TARGET one APPEND PROPERTY COMPILE_OPTIONS -Wall)
endif (UNIX)

add_custom_target(run
    COMMAND one
    DEPENDS one
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)


