cmake_minimum_required(VERSION 3.13)
project(Server)

set(CMAKE_C_STANDARD 99)

include_directories(${PROJECT_SOURCE_DIR})

#add_library(mylib STATIC Game.c)
add_executable(Server Server.c)
#target_link_libraries(Server mylib)