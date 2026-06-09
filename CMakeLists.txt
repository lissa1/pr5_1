cmake_minimum_required(VERSION 3.10)
project(FunctionGrapher)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML 3.0+
find_package(SFML 3.0 COMPONENTS graphics window system REQUIRED)

# Create executable
add_executable(function_grapher 
    src/main.cpp
    src/Graph.cpp
    src/Graph.h
)

# Link SFML libraries (SFML 3.0+ uses different linking)
target_link_libraries(function_grapher PRIVATE SFML::Graphics SFML::Window SFML::System)

# Include directories
target_include_directories(function_grapher PRIVATE src)
