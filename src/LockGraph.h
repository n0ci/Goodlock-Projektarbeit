#include "MySet.h"
#include "MyMutex.h"
#include "MyThread.h"

#include <thread>
#include <mutex>
#include <iostream>
#include <map>

class LockGraph {
    const static int MAX_MUTEX = 2;
    const static int MAX_TID = 2;
private:
    std::map<int, MySet> lockSet = std::map<int, MySet>();
    MyMutex *mutexes = new MyMutex[MAX_MUTEX];
    bool edge[MAX_MUTEX][MAX_MUTEX]{false};
    std::mutex g;

public:
    LockGraph();

    void init(MyThread **myThreads, MyMutex **myMutexes);

    void acquire(int tid, int n);

    void release(int tid, int n);

    bool check();

    void info();

    MySet directNeighbors(int node);

    bool checkCycle(int node);
};
