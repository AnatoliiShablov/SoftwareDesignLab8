cmake_minimum_required(VERSION 3.14)

project(SoftwareDesignLab8 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} main.cpp Clock.hpp Clock.cpp EventsStatistic.hpp EventsStatistic.cpp)

option(BUILD_TESTS "Build tests executable" ON)

if(BUILD_TESTS)
    add_subdirectory(Tests)
endif()
