cmake_minimum_required(VERSION 3.19)
project(Proyecto2OS C)

set(CMAKE_C_STANDARD 11)

add_executable(Proyecto2OS main.c  Scheduler/LinkedList.c Scheduler/LinkedList.h Scheduler/Scheduler.c Scheduler/Scheduler.h Synchronizer/synchronizer.c Synchronizer/synchronizer.h CEThread/CEThread.c CEThread/CEThread.h)
target_link_libraries(Proyecto2OS m confuse)