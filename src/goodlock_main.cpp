#include "goodlock_main.h"

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

int main(){
    init();

    // Kein DeadLock:
    //test_simpleLock_OneThread();

    // Potentieller Deadlock:

    //test_2Threads();                      //-> true positive      //tsan: true positive
    //test_TimeShiftDeadlock();             //-> false positive     //tsan: false positive
    //test_OnlyOneThread();                 //-> false positive     //tsan: false positive
    //test_ThreeThreads_OneCycle();         //-> true positive      //tsan: true positive
    //test_ThreeThreads_MoreCycles();       //-> true positive      //tsan: true positive
    //test_TwoThreads_3_2Locks();           //-> true positive      //tsan: true positive
    //test_TwoThreads_3Locks();             //-> false positive     //tsan: false positive

    // Sonderfälle
    //test_1Encapsulated_1Threads();        //-> false negative, erkennt keine Verkapselung von threads, malt keinen Graphen    //tsan: true negative
    //test_1Encapsulated_2Threads();        //-> false negative, erkennt keine Verkapselung von threads, malt nur Teilgraphen   //tsan: false negative

    // Deadlock:
    //test_1EncapsulatedThread_2Locks();    //-> Deadlock     //tsan: Deadlock
    LG.info();
    LG.check();
    return 0;
}

void init(){
    for(int i = 0; i < MAX_TID; i++){
        threads[i] = new MyThread(i);
    }
    for(int i = 0; i < MAX_MUTEX; i++){
        mutexes[i] = new MyMutex(i);
    }

    LG.init(threads, mutexes);
}

void lockFunction(int caseNumber, int aqFirst, int aqSecond, int threadID) {
    switch (caseNumber) {
        case 0: // zwei Locks
            LG.acquire(threadID, aqFirst);
            LG.acquire(threadID, aqSecond);
            LG.release(threadID, aqSecond);
            LG.release(threadID, aqFirst);
            break;
        case 1: // ein Lock
            LG.acquire(threadID, aqFirst);
            LG.release(threadID, aqFirst);
            break;
        default:
            break;
    }
}
void help_Function_3Locks(int aqFirst, int aqSecond, int aqThird, int threadID){
    LG.acquire(threadID, aqFirst);
    LG.acquire(threadID, aqSecond);
    LG.acquire(threadID, aqThird);
    LG.release(threadID, aqThird);
    LG.release(threadID, aqSecond);
    LG.release(threadID, aqFirst);
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
    LG.acquire(threadID, acquire);
    threads[1]->thread = std::thread(lockFunction, 1, 1, -1, 1);
    threads[1]->thread.join();
    LG.release(threadID, acquire);
}
void help_Encapsulated_2Lock_Function(int acquire, int threadID){
    LG.acquire(threadID, acquire);
    threads[1]->thread = std::thread(lockFunction, 0, 1, 0, 1);
    threads[1]->thread.join();
    LG.release(threadID, acquire);
}
void test_simpleLock_OneThread(){
    threads[0]->thread = std::thread(lockFunction, 1, 0, -1, 0);
    threads[0]->thread.join();
}
void test_OnlyOneThread() {
    threads[0]->thread = std::thread(help_OneThread_Function, 0);
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

void test_1Encapsulated_2Threads() {
    threads[0]->thread = std::thread(help_Encapsulated_Function, 0, 0);
    threads[2]->thread = std::thread(lockFunction, 0, 1, 0, 1);
    threads[0]->thread.join();
    threads[2]->thread.join();
}
void test_1EncapsulatedThread_2Locks(){
    threads[0]->thread = std::thread(help_Encapsulated_2Lock_Function, 0, 0);
    threads[0]->thread.join();
}