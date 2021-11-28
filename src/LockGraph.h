#include "MySet.h"
#include "MyMutex.h"

#include <thread>
#include <mutex>
#include <iostream>
#include <map>

class LockGraph {
    const static int MAX_MUTEX = 2;
    const static int MAX_TID = 2;
private:
    std::map<std::thread::id, MySet> lockSet = std::map<std::thread::id, MySet>();
    MyMutex *mutexes = new MyMutex[MAX_MUTEX];
    bool edge[MAX_MUTEX][MAX_MUTEX]{false};
    std::mutex g;

public:
    LockGraph();

    void init(std::thread::id TID_List[MAX_TID], MyMutex **myMutexes);

    void acquire(std::thread::id tid, int n);

    void release(std::thread::id tid, int n);

    bool check();

    void info();

    MySet directNeighbors(int node);

    bool checkCycle(int node);
};
