cmake_minimum_required(VERSION 3.15)
project(CircularBuffer)

set(CMAKE_CXX_STANDARD 14)

add_executable(CircularBuffer main.cpp src/circular_buffer.hpp src/circular_buffer_iterator.hpp src/static_circular_buffer.hpp)