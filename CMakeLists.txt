cmake_minimum_required(VERSION 3.19)
project(handler-cpp)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SRCS "src/*.cpp" "src/*.h")

add_executable(handler-cpp ${SRCS})
