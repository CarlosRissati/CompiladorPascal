cmake_minimum_required(VERSION 3.10)

project(MyCompiler)

set(CMAKE_CXX_STANDARD 17)

# Specify the source files
set(SOURCES
    src/main.cpp
    src/tokenization.hpp
    src/parser.hpp
    src/semantic_analyzer.hpp
    src/semantic_analyzer.cpp
)

# Create the executable
add_executable(MyCompiler ${SOURCES})