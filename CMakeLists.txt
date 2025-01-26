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
    ${CMAKE_SOURCE_DIR}/include/dependencies
    ${CMAKE_SOURCE_DIR}/src/Sandbox
    )

    # Check if building a shared or static library
if(BUILD_SHARED_LIBS)
    add_definitions(-Dyaml_cpp_EXPORTS)  # Define for DLL builds
else()
    add_definitions(-DYAML_CPP_STATIC_DEFINE)  # Define for static builds
endif()

# Source files
file(GLOB_RECURSE SOURCES
    ${CMAKE_SOURCE_DIR}/src/**/*.cpp
    ${CMAKE_SOURCE_DIR}/src/**/*.c
)

# Libraries directory
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Debug Editor Build
add_executable(editor_debug ${SOURCES})
target_compile_options(editor_debug PRIVATE -g)
target_compile_definitions(editor_debug PRIVATE AZIMUTH_EDITOR)
target_link_libraries(editor_debug glfw3dll assimp)
add_dependencies(editor_debug precompiled_header)
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

# Release Editor Build
add_executable(editor_release ${SOURCES})
target_compile_options(editor_release PRIVATE -O3 -g -DIMGUI_IMPL_OPENGL_LOADER_GLAD)
target_compile_definitions(editor_release PRIVATE AZIMUTH_EDITOR)
target_link_libraries(editor_release glfw3dll assimp)
add_dependencies(editor_release precompiled_header)
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

# Debug Game Build (without the editor code)
add_executable(game_debug ${SOURCES})
target_compile_options(game_debug PRIVATE -g)
target_compile_definitions(game_debug PRIVATE AZIMUTH_GAME)  # No AZIMUTH_EDITOR
target_link_libraries(game_debug glfw3dll assimp)
add_dependencies(game_debug precompiled_header)
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

# Release Game Build (without the editor code)
add_executable(game_release ${SOURCES})
target_compile_options(game_release PRIVATE -O3 -g -DIMGUI_IMPL_OPENGL_LOADER_GLAD)
target_compile_definitions(game_release PRIVATE AZIMUTH_GAME)  # No AZIMUTH_EDITOR
target_link_libraries(game_release glfw3dll assimp)
add_dependencies(game_release precompiled_header)
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
