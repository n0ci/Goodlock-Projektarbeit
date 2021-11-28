#include "LockGraph.h"


// TODO: eigene Lockmethode?
LockGraph::LockGraph() {

}

void LockGraph::init(std::thread::id TID_List[MAX_TID], MyMutex **myMutexes){
    for(int i = 0; i < MAX_TID; i++){
        lockSet.insert({TID_List[i], MySet()});
    }
}

void LockGraph::info() {
    std::cout << "\n *** INFO ***" << std::endl;
    g.lock();

    for (auto &MapEntry : lockSet) {
        std::cout << "\n Thread " << MapEntry.first << " holds the following locks:" << std::endl;
        for(int i = 0; i < MAX_MUTEX; i++){
            if (MapEntry.second.elem(i)) {
                std::cout << " " << i;
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
    g.unlock();
}

void LockGraph::acquire(std::thread::id tid, int n) {
    mutexes[n].mutex.lock();
    g.lock();
    for (int i = 0; i < MAX_MUTEX; i++) {
        if (lockSet.at(tid).elem(i)) {
            edge[i][n] = true;
        }
    }
    lockSet[tid].add(n);
    g.unlock();
}

void LockGraph::release(std::thread::id tid, int n) {
    g.lock();
    lockSet.at(tid).remove(n);
    g.unlock();
    mutexes[n].mutex.unlock();
}

MySet LockGraph::directNeighbors(int node) {
    MySet mySet;
    for (int y = 0; y < MAX_MUTEX; y++) {
        if (edge[node][y]) {
            mySet.add(y);
        }
    }
    return mySet;
}

bool LockGraph::checkCycle(int node) {
    MySet visited;
    MySet goal = directNeighbors(node);
    bool stop = false;

    while (!stop) {
        MySet new_goal;
        for (auto const&[key, val]: goal.mySet) {
            bool temp;
            std::tie(new_goal, temp) = MySet::unionSets(new_goal, directNeighbors(key));
        }
        std::tie(visited, stop) = MySet::unionSets(visited, goal);
        goal = new_goal;
    }
    if (visited.elem(node)) {
        return true;
    }
    return false;
}

bool LockGraph::check() {
    bool isCircle = false;
    g.lock();
    for (int node = 0; node < MAX_MUTEX; node++) {
        isCircle = isCircle || checkCycle(node);
    }
    g.unlock();
    if (isCircle) {
        std::cout << "\n ** cycle => potential deadlock !!! ***" << std::endl;
    }
    return isCircle;
}


