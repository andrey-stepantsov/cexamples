# An example C11 project

cmake_minimum_required(VERSION 3.15)

project(CTutor VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

enable_testing()
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
add_subdirectory(src)
add_subdirectory(tests)
# add_subdirectory(apps)
