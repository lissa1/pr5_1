cmake_minimum_required(VERSION 3.10)
project(FunctionGrapher)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Create executable
add_executable(function_grapher 
    src/main.cpp
    src/Graph.cpp
    src/Graph.h
)

# Link SFML libraries
target_link_libraries(function_grapher PRIVATE sfml-graphics sfml-window sfml-system)

# Include directories
target_include_directories(function_grapher PRIVATE src)
