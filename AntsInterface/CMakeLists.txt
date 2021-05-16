cmake_minimum_required(VERSION 3.19)
project(AntsInterface C)


set(CMAKE_C_STANDARD 11)



add_executable(AntsInterface main.c)


find_package(SDL2 REQUIRED)

include_directories("usr/include/SDL2")
target_link_libraries(AntsInterface ${SDL2_LIBRARY} -lSDL2  pthread)
target_link_libraries(AntsInterface ${SDL2_image_LIBRARY} -lSDL2_image)
target_link_libraries(AntsInterface ${SDL2_mixer_LIBRARY} -lSDL2_mixer)