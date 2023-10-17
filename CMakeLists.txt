# Minimum CMake version required
cmake_minimum_required(VERSION 3.27)

# Project information
project(CDearImGui 
    VERSION 0.2
    DESCRIPTION "A Server based approach to imgui, primarily for usage with FiveM."
    LANGUAGES C CXX
)

# Set Project Production State (toggles console)
set(IS_PROD FALSE)

# Set C & C++ standard to C++23
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Setup VCPKG link
set(VCPKG_TARGET_TRIPLET "x86-windows" CACHE STRING "")
set(CMAKE_TOOLCHAIN_FILE "D:/Github/CDearImGui/vcpkg/scripts/buildsystems/vcpkg.cmake")

# Include directories
set(INCLUDE_DIRS
    include
    include/imgui
)

# Add include directories
include_directories(${INCLUDE_DIRS})

# Gather source files
file(GLOB SOURCES 
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/**/*.cpp"
)

# Add GLFW DLL
configure_file(
    "${CMAKE_SOURCE_DIR}/dll_files/glfw3.dll"
    "${CMAKE_BINARY_DIR}/Debug/glfw3.dll"
    COPYONLY
)

# Add JSON DLL
configure_file(
    "${CMAKE_SOURCE_DIR}/dll_files/jsoncpp.dll"
    "${CMAKE_BINARY_DIR}/Debug/jsoncpp.dll"
    COPYONLY
)


# Create the executable target
if (IS_PROD)
    add_executable(CDearImGui WIN32 ${SOURCES})
else ()
    add_executable(CDearImGui ${SOURCES})
endif ()

# Find and link GLFW
set(glfw3_DIR "${CMAKE_SOURCE_DIR}/vcpkg/installed/x64-windows/share/glfw3")
find_package(glfw3 CONFIG REQUIRED)
if (glfw3_FOUND)
    target_link_libraries(CDearImGui PRIVATE glfw)
else ()
    message(FATAL_ERROR "GLFW not found!")
endif ()

# Find and link GLAD
set(glad_DIR "${CMAKE_SOURCE_DIR}/vcpkg/installed/x64-windows/share/glad")
find_package(glad CONFIG REQUIRED)
if (glad_FOUND)
    target_link_libraries(CDearImGui PRIVATE glad::glad)
else ()
    message(FATAL_ERROR "GLAD not found!")
endif ()

# Find and link Json CPP
set(jsoncpp_DIR "${CMAKE_SOURCE_DIR}/vcpkg/installed/x64-windows/share/jsoncpp")
find_package(jsoncpp CONFIG REQUIRED)
if (jsoncpp_FOUND)
    target_link_libraries(CDearImGui PRIVATE JsonCpp::JsonCpp)
else ()
    message(FATAL_ERROR "jsoncpp not found!")
endif ()

# Link Windows 32 Lib
target_link_libraries(CDearImGui PRIVATE ws2_32.lib)