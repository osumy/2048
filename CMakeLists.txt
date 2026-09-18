cmake_minimum_required(VERSION 3.20)

project(2048
    VERSION 1.0.0
    DESCRIPTION "A modern terminal-based implementation of the 2048 game in C++"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "src/*.cpp"
)

add_executable(2048 ${SOURCES})

target_include_directories(2048 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(MSVC)
    target_compile_options(2048 PRIVATE /W4 /permissive-)
else()
    target_compile_options(2048 PRIVATE -Wall -Wextra -Wpedantic)
endif()
