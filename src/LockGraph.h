#include "MySet.h"
#include "MyMutex.h"
#include "MyThread.h"

#include <thread>
#include <mutex>
#include <iostream>
#include <map>
#include <vector>
#include <iomanip>

/**
 * Die Klasse LockGraph enthält alles Notwendige, um die Funktionalitäten des GoodLock Algorithmus zur Verfügung zu stellen.
 */
class LockGraph {
    // TODO move it to main only?
    const static int MAX_MUTEX = 4;
    const static int MAX_TID = 4;
private:
    std::map<int, MySet> lockSet = std::map<int, MySet>();
    MyMutex *mutexes = new MyMutex[MAX_MUTEX];
    bool edge[MAX_MUTEX][MAX_MUTEX]{false};
    std::mutex g;
    std::vector<std::tuple<int, int, int>> history;
    int opCount = 0;


public:
    LockGraph();

    void init(MyThread **myThreads, MyMutex **myMutexes);

    void acquire(int tid, int mid);

    void release(int tid, int mid);

    bool check();

    void info();

    MySet directNeighbors(int node);

    bool checkCycle(int node);

    void printHistory();
};
