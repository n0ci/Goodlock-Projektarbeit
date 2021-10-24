#include <iostream>
#include <map>
#include <tuple>

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
    return {a, r};
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
                    new_goal = unionSets(new_goal, directNeighbors(m, key)).get(0);
                }
                visited, stop = unionSets(visited, goal);
                goal = new_goal;
            }
            if elem(visited, x)
            {
                return true;
            }
            return false;
        };

        r = false;

        lock(g);

        for (int x = 0; x < MAX_MUTEX; x++) {
            r = r || checkCycle(*m, x);
        }

        unlock(g);

        if (r) {
            std::cout << "\n ** cycle => potential deadlock !!! ***" << endl;
        }
    }


};


int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
