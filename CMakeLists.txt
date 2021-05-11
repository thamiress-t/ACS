cmake_minimum_required(VERSION 3.16)

project(ACS)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++14")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(include/control)
file(GLOB SOURCES "src/control/*.cpp")

add_executable(acs ${SOURCES} )
