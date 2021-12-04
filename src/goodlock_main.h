#include "LockGraph.h"

void init();

// Tests
void test_simpleLock_OneThread();
void test_2Threads();
void test_simple_pthread_example();
void test_TimeShiftDeadlock();
void test_OnlyOneThread();
void test_ThreeThreads_OneCycle();
void test_ThreeThreads_MoreCycles();
void test_TwoThreads_3_2Locks();
void test_TwoThreads_3Locks();
void test_1Encapsulated_1Threads();
void test_1Encapsulated_2Threads();
void test_1EncapsulatedThread_2Locks();

// Hilfsmethoden
void lockFunction(int, int, int, int);
void help_OneThread_Function(int);
void help_TimeShift_Function(int, int, int, int);
void help_Function_3Locks(int, int, int, int);
void help_Encapsulated_Function(int, int);
void help_Encapsulated_2Lock_Function(int, int);

static const int MAX_MUTEX = 4;
static const int MAX_TID = 4;

MyThread *threads [MAX_TID];
MyMutex *mutexes [MAX_MUTEX];
LockGraph LG;