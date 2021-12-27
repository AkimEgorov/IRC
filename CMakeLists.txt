cmake_minimum_required(VERSION 2.6)

project(irc)

set(CMAKE_CXX_STANDART 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread ")

include_directories("${PROJECT_SOURCE_DIR}/include")

file(GLOB
            CPP_SOURCES "src/*.cpp")
add_executable(irc ${CPP_SOURCES})
