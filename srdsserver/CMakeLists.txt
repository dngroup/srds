cmake_minimum_required(VERSION 3.9)
project(srdsserver)

set(CMAKE_CXX_STANDARD 11)

find_package (Threads)

add_executable(srdsserver main.cpp src/trackerdb.cpp src/trackerdb.h src/server.cpp src/server.h src/common_socket.cpp src/common_socket.h src/http_header.cpp src/http_header.h src/message_management.cpp src/message_management.h src/client.cpp src/client.h)

target_link_libraries (srdsserver ${CMAKE_THREAD_LIBS_INIT})