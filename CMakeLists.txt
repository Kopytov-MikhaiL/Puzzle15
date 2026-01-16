cmake_minimum_required(VERSION 4.0)
project(Puzzle15)

set(CMAKE_CXX_STANDARD 20)

add_executable(Puzzle15 main.cpp
        GameLogic.cpp
        GameMenu.cpp
        Puzzle15.h)

set(CMAKE_CXX_STANDARD 17)

# Статическая линковка для MinGW
if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

add_executable(MyApp main.cpp)

# Настройки для Release
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Release build - optimizing")
    add_compile_options(/O2)
endif()