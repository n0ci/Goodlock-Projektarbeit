#include "Set.h"

class LockGraph {
    static const int MAX_MUTEX = 2;
    static const int MAX_TID = 3;
private:
    Set lockSet[MAX_TID];
    bool edge[MAX_MUTEX][MAX_MUTEX]{false};
    std::mutex mutex[MAX_MUTEX];
    std::mutex g;

public:
    void init();

    void acquire(int tid, int n);

    void release(int tid, int n);

    bool check();

    void info();

    Set directNeighbors(int node);

    bool checkCycle(int node);
};
