cmake_minimum_required(VERSION 3.16)
project(lvgl_app C)

set(CMAKE_C_STANDARD 11)

add_compile_definitions(LV_CONF_INCLUDE_SIMPLE)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

add_subdirectory(lvgl)

add_executable(my_lvgl_app main.c)
target_include_directories(my_lvgl_app PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(my_lvgl_app PRIVATE lvgl ${SDL2_LIBRARIES})
