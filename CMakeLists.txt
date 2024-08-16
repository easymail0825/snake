cmake_minimum_required(VERSION 3.30)
project(snake)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)

include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})

add_executable(snake main.cpp
        snake.h
        square.h
        square.cpp
        snake.cpp
        food.h
        food.cpp
        scene.h
        scene.cpp
        window.h
        window.cpp
        score.h
        dialog.h
        dialog.cpp)

target_link_libraries(snake PRIVATE ${GTKMM_LIBRARIES})
