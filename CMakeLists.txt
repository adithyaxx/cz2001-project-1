cmake_minimum_required(VERSION 3.17)

project(cz2001_project_1)

set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(cz2001_project_1
        main.cpp
        src/brute_force.cpp
        src/boyer_moore.cpp
        include/utils.h
        src/utils.cpp
        src/kmp.cpp
        include/kmp.h
        include/brute_force.h
        include/boyer_moore.h
        )