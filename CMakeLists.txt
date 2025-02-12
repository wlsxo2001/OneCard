cmake_minimum_required(VERSION 3.21)
project(Onecard)

set(CMAKE_CXX_STANDARD 17)

include_directories(rule/include)

add_subdirectory(rule)
add_executable(Onecard src/main.cpp)
target_link_libraries(Onecard rule)

