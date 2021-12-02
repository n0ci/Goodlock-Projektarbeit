#include "goodlock_main.h"

void init();
void help_Function_2Locks(int number);
void test_2Threads();
void test_OneThread();
void test_OnlyOneThread();
void help_OneThread_Function();

static const int MAX_MUTEX = 2;
static const int MAX_TID = 2;

MyThread *threads [MAX_TID];
MyMutex *mutexes [MAX_MUTEX];
LockGraph LG;

int main(){
    init();
    //test_2Threads();
    test_OnlyOneThread();

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

void test_2Threads(){
    threads[0]->thread = std::thread(help_Function_2Locks, 0);
    threads[1]->thread = std::thread(help_Function_2Locks, 1);
    threads[0]->thread.join();
    threads[1]->thread.join();
}

void help_Function_2Locks(int number) {
    switch (number) {
        case 0:
            LG.acquire(threads[0]->tid, 0);
            LG.acquire(threads[0]->tid, 1);
            LG.release(threads[0]->tid, 1);
            LG.release(threads[0]->tid, 0);
            break;
        case 1:
            LG.acquire(threads[1]->tid, 1);
            LG.acquire(threads[1]->tid, 0);
            LG.release(threads[1]->tid, 0);
            LG.release(threads[1]->tid, 1);
            break;
        default:
            break;
    }
}
void help_OneThread_Function() {
    help_Function_2Locks(0);
    help_Function_2Locks(1);
}
void test_OnlyOneThread() {
    std::thread t(help_OneThread_Function);
    t.join();
}