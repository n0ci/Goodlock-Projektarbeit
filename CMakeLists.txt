cmake_minimum_required(VERSION 3.0)
project(GoodLock_Projektarbeit)

set(CMAKE_CXX_STANDARD 17)

add_executable(GoodLock_Projektarbeit tests/test_tsan_deadlock_detection.cpp src/goodlock_main.cpp src/goodlock_main.h src/Set.h src/LockGraph.cpp src/LockGraph.h)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -g -O2")