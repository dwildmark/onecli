cmake_minimum_required (VERSION 2.8.11)
project(ONE_CLI)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -std=c99 -g")
file(GLOB SOURCES "src/*.c")

include_directories(inc)

add_executable(one_cli ${SOURCES})
