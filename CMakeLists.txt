cmake_minimum_required(VERSION 3.10)
project(HomeTask2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(HomeTask2
        main.cpp
        tasks_basic.cpp
        tasks_timer.cpp
        tasks_hyperthreading.cpp
)

if(WIN32)
    target_compile_definitions(HomeTask2 PRIVATE _WIN32)
endif()