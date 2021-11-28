#include "goodlock_main.h"

void init();
void help_Function_2Locks(int number);
void test2Threads();

static const int MAX_MUTEX = 2;
static const int MAX_TID = 2;

std::thread myThreads[MAX_TID];
std::thread::id TID_List[MAX_TID];
MyMutex *mutexes [MAX_MUTEX];
LockGraph LG;

int main(){
    init();
    test2Threads();

    return 0;
}

void init(){
    for(int i = 0; i < MAX_TID; i++){
        TID_List[i] = myThreads[i].get_id();
    }
    for(int i = 0; i< MAX_MUTEX; i++){
        mutexes[i] = new MyMutex(i);
    }

    LG.init(TID_List, mutexes);
}

void test2Threads(){
    myThreads[0] = std::thread(help_Function_2Locks, 0);
    myThreads[1] = std::thread(help_Function_2Locks, 1);
    myThreads[0].join();
    myThreads[1].join();
}

void help_Function_2Locks(int number) {
    switch (number) {
        case 0:
            LG.acquire(myThreads[0].get_id(), 0);
            LG.acquire(myThreads[0].get_id(), 1);
            LG.release(myThreads[0].get_id(), 1);
            LG.release(myThreads[0].get_id(), 0);
            break;
        case 1:
            LG.acquire(myThreads[1].get_id(), 1);
            LG.acquire(myThreads[1].get_id(), 0);
            LG.release(myThreads[1].get_id(), 0);
            LG.release(myThreads[1].get_id(), 1);
            break;
        default:
            break;
    }
}