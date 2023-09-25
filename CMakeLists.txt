# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project information
project(CDearImGui 
    VERSION 0.2
    DESCRIPTION "A Server based approach to imgui, primarily for usage with FiveM."
    LANGUAGES C CXX
)

# Set C & C++ standard to C++23
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
set(INCLUDE_DIRS
    include
    include/imgui
)

# Add include directories
include_directories(${INCLUDE_DIRS})

# Gather source files
file(GLOB SOURCES 
    "src/*.cpp"
    "src/**/*.cpp"
)

# Create the executable target
add_executable(CDearImGui ${SOURCES})

# Find and link GLFW
find_package(glfw3 CONFIG REQUIRED)
if (glfw3_FOUND)
    target_link_libraries(CDearImGui PRIVATE glfw)
else ()
    message(FATAL_ERROR "GLFW not found!")
endif ()

# Find and link GLAD
find_package(glad CONFIG REQUIRED)
if (glad_FOUND)
    target_link_libraries(CDearImGui PRIVATE glad::glad)
else ()
    message(FATAL_ERROR "GLAD not found!")
endif ()

# Link Windows 32 Lib
target_link_libraries(CDearImGui PRIVATE ws2_32.lib)