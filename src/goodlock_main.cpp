#include "goodlock_main.h"

void init();


// Tests
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

int main(){
    init();

    // Potentieller Deadlock:

    //test_2Threads();
    //test_TimeShiftDeadlock();
    //test_OnlyOneThread();
    //test_ThreeThreads_OneCycle();
    //test_ThreeThreads_MoreCycles();
    //test_TwoThreads_3_2Locks();
    //test_TwoThreads_3Locks();

    // Sonderfälle
    // TODO Überprüfen
    //test_1Encapsulated_1Threads();
    //test_1Encapsulated_2Threads();

    // Deadlock:
    //test_1EncapsulatedThread_2Locks();

    LG.info();
    LG.check();
    return 0;
}

void init(){
    for(int i = 0; i < MAX_TID; i++){
        threads[i] = new MyThread(i);
    }
    for(int i = 0; i< MAX_MUTEX; i++){
        mutexes[i] = new MyMutex(i);
    }

    LG.init(threads, mutexes);
}

//aufteilen oder variable übergeben
void lockFunction(int caseNumber, int aqFirst, int aqSecond, int threadID) {

    switch (caseNumber) {
        case 0: // two Locks
            LG.acquire(threadID,aqFirst);
            LG.acquire(threadID, aqSecond);
            LG.release(threadID,aqSecond);
            LG.release(threadID,aqFirst);
            break;
        case 1: //one Lock
            LG.acquire(threadID,aqFirst);
            LG.release(threadID, aqFirst);
            break;
        default:
            break;
    }
}
void help_Function_3Locks( int aqFirst, int aqSecond, int aqThird, int threadID){
    LG.acquire(threadID,aqFirst);
    LG.acquire(threadID, aqSecond);
    LG.acquire(threadID, aqThird);
    LG.release(threadID, aqThird);
    LG.release(threadID,aqSecond);
    LG.release(threadID,aqFirst);
}
void help_OneThread_Function(int threadID) {
    lockFunction(0, 0, 1, threadID);
    lockFunction(0, 1, 0, threadID);
}
void help_TimeShift_Function(int caseNumber, int aqFirst, int aqSecond, int threadID) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    lockFunction(caseNumber, aqFirst, aqSecond, threadID);
}
void help_Encapsulated_Function(int acquire, int threadID) {
    printf("in helpFunction von t1\n");
    LG.acquire(threadID, acquire);
    threads[1]->thread = std::thread(lockFunction, 1, 1, -1, 1);
    printf("vor subaufruf\n");
    threads[1]->thread.join();
    printf("nach subaufruf\n");
    LG.release(threadID, acquire);
}
void help_Encapsulated_2Lock_Function(int acquire , int threadID){
    LG.acquire(threadID, acquire);
    threads[1]->thread = std::thread(lockFunction, 0, 1, 0, 1);
    threads[1]->thread.join();
    LG.release(threadID, acquire);
}
void test_OnlyOneThread() {
    threads[0]->thread = std::thread(help_OneThread_Function,0);
    threads[0]->thread.join();
}
void test_TimeShiftDeadlock() {
    threads[0]->thread = std::thread(lockFunction, 0, 0, 1, 0);
    threads[1]->thread = std::thread(help_TimeShift_Function, 0, 1, 0, 1);
    threads[0]->thread.join();
    threads[1]->thread.join();
}
void test_2Threads(){
    threads[0]->thread = std::thread(lockFunction, 0, 0, 1, 0);
    threads[1]->thread = std::thread(lockFunction, 0, 1, 0, 1);
    threads[0]->thread.join();
    threads[1]->thread.join();
}
void test_ThreeThreads_OneCycle() {
    threads[0]->thread = std::thread(lockFunction, 0, 0, 1, 0);
    threads[1]->thread = std::thread(lockFunction, 0, 1, 0, 1);
    threads[2]->thread = std::thread(lockFunction, 0, 2, 0, 2);
    threads[0]->thread.join();
    threads[1]->thread.join();
    threads[2]->thread.join();
}
void test_ThreeThreads_MoreCycles() {
    threads[0]->thread = std::thread(lockFunction, 0, 0, 1, 0);
    threads[1]->thread = std::thread(lockFunction, 0, 1, 0, 1);
    threads[2]->thread = std::thread(lockFunction, 0, 2, 0, 2);
    threads[3]->thread = std::thread(lockFunction, 0, 0, 2, 3);
    threads[0]->thread.join();
    threads[1]->thread.join();
    threads[2]->thread.join();
    threads[3]->thread.join();
}
void test_TwoThreads_3_2Locks() {
    threads[0]->thread = std::thread(help_Function_3Locks, 0, 1, 2, 0);
    threads[1]->thread = std::thread(lockFunction, 0, 1, 0, 1);
    threads[0]->thread.join();
    threads[1]->thread.join();
}
void test_TwoThreads_3Locks() {
    threads[0]->thread = std::thread(help_Function_3Locks, 0, 1, 2, 0);
    threads[1]->thread = std::thread(help_Function_3Locks, 0, 2, 1, 1);
    threads[0]->thread.join();
    threads[1]->thread.join();
}
void test_1Encapsulated_1Threads() {
    threads[0]->thread = std::thread(help_Encapsulated_Function, 0, 0);
    threads[0]->thread.join();
}

void test_1Encapsulated_2Threads() {        // echter Deadlock
    threads[0]->thread = std::thread(help_Encapsulated_Function, 0,0);
    threads[2]->thread = std::thread(lockFunction, 0, 1, 0, 1);
    printf("starte t1\n");
    threads[0]->thread.join();
    printf("starte t2\n");
    threads[2]->thread.join();
    printf("nach t2 \n");
}
void test_1EncapsulatedThread_2Locks(){
    threads[0]->thread = std::thread(help_Encapsulated_2Lock_Function, 0, 0);
    threads[0]->thread.join();
}