cmake_minimum_required(VERSION 3.19)
project(AntsInterface C)


set(CMAKE_C_STANDARD 11)



add_executable(AntsInterface main.c ../CEThread/CEThread.h ../CEThread/CEThread.c ../CEThread/CEThread_List.c ../CEThread/CEThread_List.h ../Scheduler/LinkedList.c ../Scheduler/LinkedList.h ../Scheduler/Scheduler.c ../Scheduler/Scheduler.h ../Synchronizer/synchronizer.c ../Synchronizer/synchronizer.h )
target_link_libraries(AntsInterface m confuse)

find_package(SDL2 REQUIRED)

include_directories("usr/include/SDL2")
target_link_libraries(AntsInterface ${SDL2_LIBRARY} -lSDL2)
target_link_libraries(AntsInterface ${SDL2_image_LIBRARY} -lSDL2_image)
target_link_libraries(AntsInterface ${SDL2_mixer_LIBRARY} -lSDL2_mixer)