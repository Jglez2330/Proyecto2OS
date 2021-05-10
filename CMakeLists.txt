cmake_minimum_required(VERSION 3.19)
project(Proyecto2OS C)

set(CMAKE_C_STANDARD 11)

add_executable(Proyecto2OS main.c CEThread.c CEThread.h Scheduler/SchedulingAlgorithms.c Scheduler/SchedulingAlgorithms.h Scheduler/ThreadsQueue.c Scheduler/ThreadsQueue.h)