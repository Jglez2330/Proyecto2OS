cmake_minimum_required(VERSION 3.19)
project(Proyecto2OS C)

set(CMAKE_C_STANDARD 11)

add_executable(Proyecto2OS main.c CEThread/CEThread.c CEThread/CEThread.h CEThread/CEThread_List.c CEThread/CEThread_List.h)
target_link_libraries(Proyecto2OS m confuse)