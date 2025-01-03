cmake_minimum_required(VERSION 3.10)
project(console-renderer C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

# Add include directory
include_directories(include)

# Add executable
file(GLOB_RECURSE SRC_FILES src/*.c)
add_executable(game ${SRC_FILES})

# Find and link required libraries
target_link_libraries(game m)  # Link math library

# Set compiler warnings and optimization level
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(game PRIVATE -Wall -Wextra -Wpedantic)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(game PRIVATE -O3)
    endif()
endif()

# Include directories
target_include_directories(game PRIVATE ${CMAKE_SOURCE_DIR}/include)