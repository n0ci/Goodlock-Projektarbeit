#include <iostream>
#include <map>
#include <tuple>

// TODO: get locks/mutex to work, refactor with some OOP, fix several de/reference bugs, debug, examples, tests?


class Set {
public:
    std::map<char, int> mySet = std::map<char, int>();
};

Set mkSet() {
    return Set();
}

bool empty(Set set) {
    return set.mySet.empty();
}

bool elem(Set set, int n) {
    return set.mySet[n];
}

Set add(Set set, int n) {
    auto s = set;
    s.mySet[n] = true;
    return s;
}

std::tuple<Set, bool> unionSets(Set a, Set b) {
    auto r = true;
    for (auto const&[key, val]: b.mySet) {
        if (!a.mySet.count(key)) {
            r = false;
            a.mySet.insert({key, val});
        }
    }
    return std::make_tuple(a, r);
}

Set remove(Set set, int n) {
    auto s = set;
    s.mySet.erase(n);
    return s;
}

// Mutex

// TODO: implement an actual mutex using c++ libs, meanwhile mocking
class Mutex {
};

Mutex newMutex() {
    Mutex m;
    return m;
}

void lock(Mutex m);

void unlock(Mutex m);


class Userinterface {
public:
    virtual void init();

    virtual void acquire(int tid, int n);

    virtual void release(int tid, int n);

    virtual bool check();

    virtual void info();

    virtual ~Userinterface() = default;
};

const int MAX_MUTEX = 3;
const int MAX_TID = 4;

class LG {
private:
    Set lockset[MAX_TID];
    bool edge[MAX_MUTEX][MAX_MUTEX];
    Mutex mutex[MAX_MUTEX];
    Mutex g;

public:
    void init() {
        for (int i = 0; i < MAX_TID; i++) {
            lockset[i] = mkSet();
        }
        for (int i = 0; i < MAX_MUTEX; i++) {
            for (int j = 0; j < MAX_MUTEX; j++) {
                edge[i][j] = false;
            }
        }
        for (int i = 0; i < MAX_MUTEX; i++) {
            mutex[i] = newMutex();
        }
        g = newMutex();
    }

    void info() {
        std::cout << "\n *** INFO ***" << std::endl;
        lock(g);

        for (int i = 0; i < MAX_TID; i++) {
            std::cout << "\n Thread " << i << " holds the following locks:" << std::endl;
            for (int j = 0; j < MAX_MUTEX; j++) {
                if (elem(lockset[i], j)) {
                    std::cout << " " << j;
                }
            }
        }
        std::cout << "\n Lock graph:";
        for (int i = 0; i < MAX_MUTEX; i++) {
            for (int j = 0; j < MAX_MUTEX; j++) {
                if (edge[i][j]) {
                    std::cout << "\n " << i << " --> " << j;
                }
            }
        }
        unlock(g);
    }

    void acquire(int tid, int n) {
        lock(mutex[n]);
        lock(g);
        for (int i = 0; i < MAX_MUTEX; i++) {
            if (elem(lockset[tid], i)) {
                edge[i][n] = true;
            }
        }
    }

    void release(int tid, int n) {
        lock(g);
        lockset[tid] = remove(lockset[tid], n);
        unlock(g);
        unlock(mutex[n]);
    }

    bool check() {
        auto directNeighbors = [this](LG m, int x) -> Set {
            auto next = mkSet();
            for (int y = 0; y < MAX_MUTEX; y++) {
                if (edge[x][y]) {
                    next = add(next, y);
                }
            }
            return next;
        };

        auto checkCycle = [&directNeighbors](LG m, int x) -> bool {
            auto visited = mkSet();
            auto goal = directNeighbors(m, x);
            auto stop = false;

            while (!stop) {
                auto new_goal = mkSet();
                for (auto const&[key, val]: goal.mySet) {
                    bool temp;
                    std::tie(new_goal, temp) = unionSets(new_goal, directNeighbors(m, key));
                }
                std::tie(visited, stop) = unionSets(visited, goal);
                goal = new_goal;
            }
            if (elem(visited, x)) {
                return true;
            }
            return false;
        };

        auto r = false;

        lock(g);

        for (int x = 0; x < MAX_MUTEX; x++) {
            r = r || checkCycle(*this, x);
        }

        unlock(g);

        if (r) {
            std::cout << "\n ** cycle => potential deadlock !!! ***" << std::endl;
        }
    }
};


// Examples

void example1(LG m){
    auto x = 0;
    auto y = 1;
    auto t0 = 0;
    auto t1 = 1;

    m.init();

    auto acqRel2 = [&m](int t, int a, int b){
        m.acquire(t, a);
        m.acquire(t, b);
        m.release(t, b);
        m.release(t, a);
    };

    acqRel2(t0, x, y);
    acqRel2(t1, y, x);

    m.info();
    m.check();
}

void example2(LG m){
    auto x = 0;
    auto y = 1;
    auto t0 = 0;
    auto t1 = 1;

    m.init();

    // TODO: find Go channel alternative and implement accordingly

    auto ch = 0;

    auto acqRel2 = [&m](int t, int a, int b){
        m.acquire(t, a);
        m.acquire(t, b);
        m.release(t, b);
        m.release(t, a);
    };

    // TODO: find Goroutine alternative and implement accordingly
    acqRel2(t0, x, y);
    ch = 1;

    ch = 0;

    acqRel2(t1, y, x);

    m.info();
    m.check();
}

void example3(LG m){
    auto x = 0;
    auto y = 1;
    auto z = 2;

    auto t0 = 0;
    auto t1 = 1;
    auto t2 = 2;

    m.init();

    auto acqRel2 = [&m](int t, int a, int b){
        m.acquire(t, a);
        m.acquire(t, b);
        m.release(t, b);
        m.release(t, a);
    };

    // TODO: Goroutine

    acqRel2(t0, x, y);
    acqRel2(t1, y, z);
    acqRel2(t2, z, x);

    m.info();
    m.check();

    // TODO: find possibility to sleep, if necessary
    // sleep 1 sec here
}

void example4(LG m){
    auto x = 0;
    auto y = 1;
    auto z = 2;

    auto t0 = 0;
    auto t1 = 1;

    m.init();

    // TODO: Goroutine
    m.acquire(t0, x);
    m.acquire(t0, y);
    m.acquire(t0, z);
    m.release(t0, z);
    m.release(t0, y);
    m.release(t0, x);

    m.acquire(t1, z);
    m.acquire(t1, x);
    m.release(t1, x);
    m.release(t1, z);

    // TODO: find possibility to sleep, if necessary
    // sleep 1 sec here
}

void example5(LG m){
    auto x = 0;
    auto y = 1;
    auto z = 2;

    auto t0 = 0;
    auto t1 = 1;

    // TODO: Goroutine
    m.acquire(t0, x);
    m.acquire(t0, y);
    m.acquire(t0, z);
    m.release(t0, z);
    m.release(t0, y);
    m.release(t0, x);

    m.acquire(t1, x);
    m.acquire(t1, z);
    m.acquire(t1, y);
    m.release(t1, y);
    m.release(t1, z);
    m.release(t1, x);

    m.info();
    m.check();

    // TODO: find possibility to sleep, if necessary
    // sleep 1 sec here
}



int main() {
    LG lg;
    // example1(lg);
    // example2(lg);
    // example3(lg);
    // example4(lg);
    example5(lg);
    return 0;
}