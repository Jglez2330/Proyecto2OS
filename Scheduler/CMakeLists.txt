cmake_minimum_required(VERSION 3.19)
project(Scheduler C)

set(CMAKE_C_STANDARD 11)

add_executable(Scheduler mainSheduler.c ThreadsQueue.h ThreadsQueue.c Scheduler.c Scheduler.h LinkedList.c LinkedList.h)