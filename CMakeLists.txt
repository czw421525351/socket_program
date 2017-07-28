cmake_minimum_required(VERSION 3.7)
project(socket_program)

set(CMAKE_CXX_STANDARD 11)

add_executable(01byteorder 01byteorder.cpp)

add_executable(02addr 02addr.cpp)

add_executable(03echo_server 03echo_server.cpp)
add_executable(03echo_client 03echo_client.cpp)

