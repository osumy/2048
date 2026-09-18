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

if(WIN32)
    target_link_libraries(2048 PRIVATE winmm)
    add_custom_command(TARGET 2048 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/mondamusic-8-bit-retro.mp3"
        "$<TARGET_FILE_DIR:2048>/mondamusic-8-bit-retro.mp3"
        COMMENT "Copying background music to target directory"
    )
endif()

