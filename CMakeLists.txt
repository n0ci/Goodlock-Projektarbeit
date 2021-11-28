cmake_minimum_required(VERSION 3.0)
project(GoodLock_Projektarbeit)

set(CMAKE_CXX_STANDARD 17)

add_executable(GoodLock_Projektarbeit src/goodlock_main.cpp src/goodlock_main.h src/MySet.h src/LockGraph.cpp src/LockGraph.h src/MyMutex.h)

SET(CMAKE_CXX_FLAGS -pthread)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -g -O2")