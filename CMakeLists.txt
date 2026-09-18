cmake_minimum_required(VERSION 3.20)

project(2048
    VERSION 1.1.0
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

set(EXTRA_SOURCES "")
if(WIN32)
    enable_language(RC)
    list(APPEND EXTRA_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/resources.rc")
endif()

add_executable(2048 ${SOURCES} ${EXTRA_SOURCES})

target_include_directories(2048 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(MSVC)
    target_compile_options(2048 PRIVATE /W4 /permissive-)
else()
    target_compile_options(2048 PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(WIN32)
    target_link_libraries(2048 PRIVATE winmm)
endif()

