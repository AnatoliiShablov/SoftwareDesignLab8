cmake_minimum_required(VERSION 3.14)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.17.0/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
                EXPECTED_HASH SHA256=3bef79da16c2e031dc429e1dac87a08b9226418b300ce004cc125a82687baeef
                TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

project(SoftwareDesignLab8 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(EventsStatistic include/Clock.hpp           src/Clock.cpp
                            include/EventsStatistic.hpp src/EventsStatistic.cpp
)
target_include_directories(EventsStatistic PRIVATE include)
target_include_directories(EventsStatistic INTERFACE include)

option(BUILD_TESTS "Build tests executable" ON)

if(BUILD_TESTS)
    add_subdirectory(Tests)
endif()
