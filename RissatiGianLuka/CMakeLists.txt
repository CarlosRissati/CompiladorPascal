cmake_minimum_required(VERSION 3.10)

project(PascalCompiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(compiler 
    src/main.cpp
    src/analisador_semantico.cpp 
)

target_include_directories(compiler PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)