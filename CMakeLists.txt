cmake_minimum_required(VERSION 3.15)
project(Azimuth)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define the compiler (adjust path if needed)
set(CMAKE_CXX_COMPILER "D:/MinGW/bin/g++.exe")

# Precompiled Header Function
function(setup_precompiled_headers target pch_file output_file)
    add_custom_command(
        OUTPUT ${output_file}
        COMMAND ${CMAKE_CXX_COMPILER} -x c++-header -std=c++17 -I${CMAKE_SOURCE_DIR}/include ${pch_file} -o ${output_file}
        DEPENDS ${pch_file}
    )
    add_custom_target(${target} ALL DEPENDS ${output_file})
endfunction()

# Precompiled Header Setup
setup_precompiled_headers(precompiled_header ${CMAKE_SOURCE_DIR}/include/Azimuth/Common.h ${CMAKE_SOURCE_DIR}/include/Azimuth/Common.h.gch)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/Sandbox
    )

# Source files
file(GLOB_RECURSE SOURCES
    ${CMAKE_SOURCE_DIR}/src/**/*.cpp
    ${CMAKE_SOURCE_DIR}/src/**/*.c
)

# Libraries directory
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Debug Build
add_executable(main_debug ${SOURCES})
target_compile_options(main_debug PRIVATE -g)
target_link_libraries(main_debug glfw3dll assimp)
add_dependencies(main_debug precompiled_header)

# Release Build
add_executable(main_release ${SOURCES})
target_compile_options(main_release PRIVATE -O3 -DIMGUI_IMPL_OPENGL_LOADER_GLAD)
target_link_libraries(main_release glfw3dll assimp)
add_dependencies(main_release precompiled_header)
