cmake_minimum_required(VERSION 3.0)
project(GoodLock_Projektarbeit)

set(CMAKE_CXX_STANDARD 17)

add_executable(GoodLock_Projektarbeit src/MySet.h src/LockGraph.cpp src/LockGraph.h src/MyMutex.h src/MyThread.h tests/test_my_deadlock_detection.cpp)
SET(CMAKE_CXX_FLAGS -pthread)

#add_executable(GoodLock_Projektarbeit tests/test_tsan_deadlock_detection.cpp)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -g -O2")