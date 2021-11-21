#include <iostream>
#include "LockGraph.h"

// TODO: eigene Lockmethode?
void LockGraph::init() {

}

void LockGraph::info() {
    std::cout << "\n *** INFO ***" << std::endl;
    g.lock();

    for (int i = 0; i < MAX_TID; i++) {
        std::cout << "\n Thread " << i << " holds the following locks:" << std::endl;
        for (int j = 0; j < MAX_MUTEX; j++) {
            if (lockSet[i].elem(j)) {
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
    g.unlock();
}

void LockGraph::acquire(int tid, int n) {
    mutex[n].lock();
    g.lock();
    for (int i = 0; i < MAX_MUTEX; i++) {
        if (lockSet[tid].elem(i)) {
            edge[i][n] = true;
        }
    }
}

void LockGraph::release(int tid, int n) {
    g.lock();
    lockSet[tid].remove(n);
    g.unlock();
    mutex[n].unlock();
}

Set LockGraph::directNeighbors(int node) {
    Set mySet;
    for (int y = 0; y < MAX_MUTEX; y++) {
        if (edge[node][y]) {
            mySet.add(y);
        }
    }
    return mySet;
}

bool LockGraph::checkCycle(int node) {
    Set visited;
    Set goal = directNeighbors(node);
    bool stop = false;

    while (!stop) {
        Set new_goal;
        for (auto const&[key, val]: goal.mySet) {
            bool temp;
            std::tie(new_goal, temp) = Set::unionSets(new_goal, directNeighbors(key));
        }
        std::tie(visited, stop) = Set::unionSets(visited, goal);
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
