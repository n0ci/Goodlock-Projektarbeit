#include "LockGraph.h"

/**
 * Todo: init entfernen und Constructor nehmen?
 */
LockGraph::LockGraph() = default;

/**
 * Initialisiert die nötigen Threads und Mutexes, welche von außen übergeben werden.
 * Es wird über die Threads iteriert und diese jeweils mit ihrer TID als Key in der Map (lockSet) eingefügt.
 * Die Value für jeden Key (TID) ist ein Objekt der Klasse MySet.
 * Weiter wird für jede Value (MySet) einer TID in die dort enthaltene, zweite, Map mit <Mutex ID, boolean>
 * jeder Wert auf false gesetzt, weil bisher noch kein Mutex erworben wurde.
 * @param myThreads Array vom Typ MyThread, enthält Threads mit TID.
 * @param myMutexes Array vom Typ MyMutex, enthält Mutexe mit MID.
 */
void LockGraph::init(MyThread **myThreads, MyMutex **myMutexes){
    for(int i = 0; i < MAX_TID; i++){
        lockSet.insert({myThreads[i]->tid, MySet()});
        for(int y = 0; y < MAX_MUTEX; y++){
            lockSet.at(i).mySet.insert({myMutexes[y]->mid, false});
        }
    }
}

/**
 * Gibt in der Kommandozeile Auskunft darüber, welcher Thread, welche Locks hält und wie der Lock Graph aussieht.
 * Um nennenswerte Ergebnisse zu sehen, sollte diese Funktion nicht nur am Ende ausgeführt werden.
 */
void LockGraph::info() {
    std::cout << "\n *** INFO ***";
    g.lock();

    for (auto &MapEntry : lockSet) {
        std::cout << "\n Thread " << MapEntry.first << " holds the following locks:";
        for(int i = 0; i < MAX_MUTEX; i++){
            if (MapEntry.second.elem(i)) {
                std::cout << " " << i;
            }
        }
    }

    std::cout << "\n\n Lock graph:";
    for (int i = 0; i < MAX_MUTEX; i++) {
        for (int j = 0; j < MAX_MUTEX; j++) {
            if (edge[i][j]) {
                std::cout << "\n " << i << " --> " << j;
            }
        }
    }
    g.unlock();
}


/**
 * Erwirbt mit einem bestimmten Thread, identifiziert durch die TID, ein bestimmtes Mutex, identifiziert durch MID.
 * @param tid Die ID des Threads.
 * @param mid Die ID des Mutex.
 */
void LockGraph::acquire(int tid, int mid) {
    mutexes[mid].mutex.lock();
    g.lock();
    for (int i = 0; i < MAX_MUTEX; i++) {
        if (lockSet.at(tid).elem(i)) {
            edge[i][mid] = true;
        }
    }
    lockSet.at(tid).add(mid);
    g.unlock();
}

/**
 * Gibt ein bestimmtes Mutex, identifiziert durch MID, auf einem bestimmten Thread, identifiziert durch TID, wieder frei.
 * @param tid Die ID des Threads.
 * @param mid Die ID des Mutex.
 */
void LockGraph::release(int tid, int mid) {
    g.lock();
    lockSet.at(tid).remove(mid);
    g.unlock();
    mutexes[mid].mutex.unlock();
}

/**
 * Sucht aktiv nach direkten Nachbarn im Kantengraph und gibt ein Objekt MySet mit diesen Nachbarn zurück.
 * @param node Der Knoten von dem aus die Nachbarn gesucht werden, ferner die MID.
 * @return Objekt vom Typ MySet mit den entsprechenden Nachbarn.
 */
MySet LockGraph::directNeighbors(int node) {
    MySet mySet;
    for (int y = 0; y < MAX_MUTEX; y++) {
        if (edge[node][y]) {
            mySet.add_unsafe(y);
        }
    }
    return mySet;
}

/**
 * Prüft einen Knoten auf Zyklen.
 * Schritt 1: Die direkten Nachbarn eines Knotens (node) in goal speichern.
 * Schritt 2: Die direkten Nachbarn, jedes Knotens in goal mit new_goal vereinigen.
 * Schritt 3: Alle knoten in goal, werden mit denen von visited vereinigt und dort gespeichert, über diese wurde dann
 *            bereits iteriert. Da unionSets true ausgibt, wenn Mengengleichheit herrscht, wird dieser Wert nun in stop
 *            gespeichert und definiert unsere Abbruchbedingung.
 * Schritt 4: Jetzt werden die Knoten von goal mit den neuen Nachbarknoten von new_goal überschrieben.
 * Schritt 5: Es wird geprüft, ob der Startknoten in den Knoten von visited vorkommt.
              Ist dies der Fall, gibt es einen Zyklus.
 * @param node Der Knoten, welcher geprüft werden soll.
 * @return Wenn ein Zyklus gefunden wurde, wird true zurückgegeben. Andernfalls false.
 */
bool LockGraph::checkCycle(int node) {
    MySet visited;
    MySet goal = directNeighbors(node);
    bool stop = false;

    while(!stop) {
        MySet new_goal;
        for (auto const&[key, val]: goal.mySet) {
            bool temp;
            std::tie(new_goal, temp) = MySet::unionSets(new_goal, directNeighbors(key));
        }
        std::tie(visited, stop) = MySet::unionSets(visited, goal);
        goal = new_goal;
    }
    if (visited.elem_unsafe(node)) {
        return true;
    }
    return false;
}

/**
 * Überprüft, ob Zyklen existieren, falls dem so ist, gibt es eine Textausgabe mit Warnung, dass ein potentieller
 * Deadlock vorhanden ist.
 * @return True, wenn mindestens ein Zyklus vorhanden ist. False, wenn frei von Zyklen.
 */
bool LockGraph::check() {
    bool isCircle = false;
    g.lock();
    for (int node = 0; node < MAX_MUTEX; node++) {
        isCircle = isCircle || checkCycle(node);
    }
    g.unlock();
    if (isCircle) {
        std::cout << "\n *** cycle => potential deadlock !!! ***" << std::endl;
    }
    return isCircle;
}


