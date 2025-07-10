cmake_minimum_required(VERSION 3.10)

project(MyCompiler)

set(CMAKE_CXX_STANDARD 17)

# Specify the source files
set(SOURCES
    src/main.cpp
    src/parser.hpp
    src/tokenization.hpp
    src/semantic_analyzer.cpp
    src/semantic_analyzer.hpp
    src/ast.hpp
)

# Create the executable
add_executable(MyCompiler ${SOURCES})

# Include directories
target_include_directories(MyCompiler PRIVATE src)