cmake_minimum_required(VERSION 3.13)
project(Server)

set(CMAKE_C_STANDARD 99)

include_directories(${PROJECT_SOURCE_DIR})
add_executable(Client)  # be sure there's exactly one main() function in the source files