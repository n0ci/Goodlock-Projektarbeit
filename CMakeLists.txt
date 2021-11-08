cmake_minimum_required(VERSION 3.0)
project(GoodLock_Projektarbeit)

set(CMAKE_CXX_STANDARD 14)

add_executable(GoodLock_Projektarbeit tests/test_tsan_deadlock_detection.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -g -O2")