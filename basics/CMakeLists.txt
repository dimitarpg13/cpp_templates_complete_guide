cmake_minimum_required(VERSION 3.20)

# set the project name
project(CppTemplatesBasics VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(max1 max1.cpp)
add_executable(maxauto maxauto.cpp)
add_executable(maxdecltype maxdecltype.cpp)
add_executable(maxdecltypedecay maxdecltypedecay.cpp)
add_executable(maxcommon maxcommon.cpp)
add_executable(maxdefault1 maxdefault1.cpp)
add_executable(maxdefaultdeclval maxdefaultdeclval.cpp)
