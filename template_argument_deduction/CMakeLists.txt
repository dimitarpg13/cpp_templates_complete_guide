cmake_minimum_required(VERSION 3.20)

# set the project name
project(CppTemplateArgumentDeduction VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(fppm details/fppm.cpp)
