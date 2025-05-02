cmake_minimum_required(VERSION 3.10)

# Project name
project(MyProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm/bin/clang++")
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CPPBIN_DIR /Users/achyutpujar/opt/CppBin)

# Add executable
add_executable(MyExecutable Day2/day2_bookstore.cpp)

target_include_directories(MyExecutable PRIVATE
    ${CPPBIN_DIR}/json/include
    ${CPPBIN_DIR}/httplib-0.20.0
)