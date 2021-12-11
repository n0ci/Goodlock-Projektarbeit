#include "../src/LockGraph.h"

// Tests
void test_simpleLock_OneThread();

void test_2Threads();

void test_simple_pthread_example();

void test_TimeShiftDeadlock();

void test_OnlyOneThread();

void test_ThreeThreads_OneCycle();

void test_FourThreads_MoreCycles();

void test_TwoThreads_3_2Locks();

void test_TwoThreads_3Locks();

void test_1Encapsulated_1Threads();

void test_1Encapsulated_2Threads();

void test_1EncapsulatedThread_2Locks();

// Hilfsmethoden
void lockFunction(int, int, int, int);

void help_OneThread_Function(int);

void help_TimeShift_Function(int, int, int, int);

void help_Function_3Locks(int, int, int, int);

void help_Encapsulated_Function(int, int);

void help_Encapsulated_2Lock_Function(int, int);

void init();

static const int MAX_MUTEX = 4;
static const int MAX_TID = 4;

MyThread *threads[MAX_TID];
MyMutex *mutexes[MAX_MUTEX];
LockGraph LG = LockGraph();

/**
 * Die Main Methode ist der Eintrittspunkt des Programms.
 * @return default Wert 0.
 */
int main() {
    init();

    // Fälle für keine potenzielle DeadLocks:

    //test_simpleLock_OneThread();          // -> true negative     //tsan: true negative
    //test_TimeShiftDeadlock();             //-> false positive     //tsan: false positive
    //test_OnlyOneThread();                 //-> false positive     //tsan: false positive
    //test_TwoThreads_3Locks();             //-> false positive     //tsan: false positive

    // Fälle für potenzielle Deadlocks:

    //test_2Threads();                      //-> true positive      //tsan: true positive
    //test_ThreeThreads_OneCycle();         //-> true positive      //tsan: true positive
    test_FourThreads_MoreCycles();        //-> true positive      //tsan: true positive
    //test_TwoThreads_3_2Locks();           //-> true positive      //tsan: true positive

    // Fälle mit verschachtelten Threads:

    //test_1Encapsulated_1Threads();        //-> true negative, jedoch erkennt es keine Verschachtelung von Threads, malt keinen Graphen    //tsan: true negative
    //test_1Encapsulated_2Threads();        //-> false negative, erkennt keine Verschachtelung von Threads, malt nur Teilgraphen            //tsan: false negative

    // Fall für einen tatsächlichen Deadlock:

    //test_1EncapsulatedThread_2Locks();    //-> Deadlock     //tsan: Deadlock

    LG.info();
    LG.check();
    LG.printHistory();
    return 0;
}

/**
 * Initialisiert Threads und Locks mit TID und MID. Führt anschließend die Initialisierung des LG aus.
 */
void init() {
    for (int i = 0; i < MAX_TID; i++) {
        threads[i] = new MyThread(i);
    }
    for (int i = 0; i < MAX_MUTEX; i++) {
        mutexes[i] = new MyMutex(i);
    }
    LG.init(threads, mutexes);
}

/**
 * Diese Testmethode testet, ob eine Lockreihenfolge mit einem Mutex auf einem Thread funktioniert.
 * Dieser Thread produziert kein Deadlock.
 * true negative.
 * Aufruf mit Thread 0:     Lock: 0         Unlock: 0.
 */
void test_simpleLock_OneThread() {
    threads[0]->thread = std::thread(lockFunction, 1, 0, -1, 0);
    threads[0]->thread.join();
}

/**
 * Diese Testmethode testet, ob zwei Lockreihenfolgen mit zwei Mutexen auf einem Thread als potenziellen Deadlock erkennt werden.
 * Dieser eine Thread kann keinen Deadlock produzieren.
 * false positive.
 * Aufruf mit Thread 0:     Lock: 0 -> 1     Unlock: 1 -> 0.
 * Aufruf mit Thread 0:     Lock: 1 -> 0     Unlock: 0 -> 1.
 */
void test_OnlyOneThread() {
    threads[0]->thread = std::thread(help_OneThread_Function, 0);
    threads[0]->thread.join();
}

/**
 * Diese Testmethode testet, ob zwei zeitversetzte Lockreihenfolgen auf zwei Threads trotzdem als potenziellen Deadlock wahrgenommen werden.
 * Obwohl das nie zu einem Deadlock führen kann, da Thread 1 erst anfängt, wenn Thread 0 schon lange fertig ist.
 * false positive.
 * Aufruf mit Thread 0:     Lock: 0 -> 1,    Unlock: 1 -> 0.
 * Thread 1 schläft.
 * Aufruf mit Thread 1:     Lock: 1 -> 0,    Unlock: 0 -> 1.
 *
 */
void test_TimeShiftDeadlock() {
    threads[0]->thread = std::thread(lockFunction, 0, 0, 1, 0);
    threads[1]->thread = std::thread(help_TimeShift_Function, 0, 1, 0, 1);
    threads[0]->thread.join();
    threads[1]->thread.join();
}

/**
 * Diese Testmethode testet einen klassischen Deadlock mit zwei Threads und zwei Mutexen.
 * Es wird ein potenziellen Deadlock erkannt.
 * true positive.
 * Aufruf mit Thread 0:    Lock: 0 -> 1    Unlock: 1 -> 0.
 * Aufruf mit Thread 1:    Lock: 1 -> 0    Unlock: 0 -> 1.
 */
void test_2Threads() {
    threads[0]->thread = std::thread(lockFunction, 0, 0, 1, 0);
    threads[1]->thread = std::thread(lockFunction, 0, 1, 0, 1);
    threads[0]->thread.join();
    threads[1]->thread.join();
}

/**
 * Diese Testmethode testet, ob ein Zyklus mit mehr als nur zwei Threads erkannt wird.
 * Es wird ein potenziellen Deadlock bei Thread 0 und Thread 1 erkannt.
 * true positive.
 * Aufruf mit Thread 0:     Lock: 0 -> 1    Unlock: 1 -> 0.
 * Aufruf mit Thread 1:     Lock: 1 -> 0    Unlock: 0 -> 1.
 * Aufruf mit Thread 2:     Lock: 2 -> 0    Unlock: 0 -> 2.
 */
void test_ThreeThreads_OneCycle() {
    threads[0]->thread = std::thread(lockFunction, 0, 0, 1, 0);
    threads[1]->thread = std::thread(lockFunction, 0, 1, 0, 1);
    threads[2]->thread = std::thread(lockFunction, 0, 2, 0, 2);
    threads[0]->thread.join();
    threads[1]->thread.join();
    threads[2]->thread.join();
}

/**
 * Diese Testmethode testet, ob mehrere Zyklen mit mehr als nur zwei Threads erkannt werden.
 * Es werden zwei potenzielle Deadlocks bei Thread 0 und Thread 1 und bei Thread 2 und Thread 3 erkannt.
 * true positive.
 * Aufruf mit Thread 0:     Lock: 0 -> 1    Unlock: 1 -> 0.
 * Aufruf mit Thread 1:     Lock: 1 -> 0    Unlock: 0 -> 1.
 * Aufruf mit Thread 2:     Lock: 2 -> 0    Unlock: 0 -> 2.
 * Aufruf mit Thread 3:     Lock: 0 -> 2    Unlock: 2 -> 0.
 */
void test_FourThreads_MoreCycles() {
    threads[0]->thread = std::thread(lockFunction, 0, 0, 1, 0);
    threads[1]->thread = std::thread(lockFunction, 0, 1, 0, 1);
    threads[2]->thread = std::thread(lockFunction, 0, 2, 0, 2);
    threads[3]->thread = std::thread(lockFunction, 0, 0, 2, 3);
    threads[0]->thread.join();
    threads[1]->thread.join();
    threads[2]->thread.join();
    threads[3]->thread.join();
}

/**
 * Diese Testmethode testet, ob ein potenzieller Deadlock mit zwei Treads und unterschiedlichen Anzahl an Mutexen erkannt wird.
 * Es wird ein potenziellen Deadlock erkannt.
 * true positive.
 * Aufruf mit Thread 0:     Lock: 0 -> 1 -> 2   Unlock: 2 -> 1 -> 2.
 * Aufruf mit Thread 1:     Lock: 1 -> 0        Unlock: 0 -> 1.
 */
void test_TwoThreads_3_2Locks() {
    threads[0]->thread = std::thread(help_Function_3Locks, 0, 1, 2, 0);
    threads[1]->thread = std::thread(lockFunction, 0, 1, 0, 1);
    threads[0]->thread.join();
    threads[1]->thread.join();
}

/**
 * Diese Testmethode testet, ob ein nicht potenzieller Deadlock mit zwei Threads und einem gleichen Startmutex erkannt wird.
 * Es wird ein potenzieller Deadlock wegen Thread 0: 1->2 und Thread 1: 2->1 erkannt.
 * Jedoch kann das nie zu einem Deadlock führen, da bevor diese Mutexe gelocked werden können, muss das Mutex 0 gelocked sein.
 * Und dieses kann immer nur von einem Thread gelocked sein und der andere Thread muss warten.
 * false positive
 * Aufruf mit Thread 0:     Lock: 0 -> 1 -> 2   Unlock: 2 -> 1 -> 0.
 * Aufruf mit Thread 1:     Lock: 0 -> 2 -> 1   Unlock: 1 -> 2 -> 0.
 */
void test_TwoThreads_3Locks() {
    threads[0]->thread = std::thread(help_Function_3Locks, 0, 1, 2, 0);
    threads[1]->thread = std::thread(help_Function_3Locks, 0, 2, 1, 1);
    threads[0]->thread.join();
    threads[1]->thread.join();
}

/**
 ** Diese Testmethode testet, ob kein potenzieller Deadlock mit einen verschachtelten Thread mit jeweils einem Mutex erkannt wird.
 * true negative, jedoch wird keine Abhängigkeiten zwischen den verschachtelten Threads erkannt.
 * Aufruf mit Thread 0:         Lock: 0 und starte Thread subThread       Unlock: 0.
 * Aufruf mit Thread subThread: Lock: 1                                   Unlock: 1
 */
void test_1Encapsulated_1Threads() {
    threads[0]->thread = std::thread(help_Encapsulated_Function, 0, 0);
    threads[0]->thread.join();
}

/**
 * Diese Testmethode testet, ob ein potenzieller Deadlock bei einem verschachtelten und einem normalen Thread mit verschiedener Anzahl an Lockreihenfolgen erkannt wird.
 * Die Verschachtelung wird nicht erkannt.
 * false negative.
 * Aufruf mit Thread 1:         Lock: 0 und starte Thread subThread       Unlock: 0.
 * Aufruf mit Thread subThread: Lock: 1                                   Unlock: 1.
 * Aufruf mit Thread 2:         Lock: 1 -> 0                              Unlock: 0 -> 1.
 */
void test_1Encapsulated_2Threads() {
    threads[0]->thread = std::thread(help_Encapsulated_Function, 0, 0);
    threads[2]->thread = std::thread(lockFunction, 0, 1, 0, 1);
    threads[0]->thread.join();
    threads[2]->thread.join();
}

/**
 * Diese Testmethode testet, ob ein Deadlock bei einem verschachtelten Thread mit verschiedener Lockreihenfolge erkannt wird.
 * Es wird ein Deadlock erzeugt und so kann der Algorithmus diesen nicht erkennen, da dass Programm sich aufhängt.
 * Deadlock.
 * Aufruf mit Thread 1:         Lock: 0 und starte Thread subThread       Unlock: 0.
 * Aufruf mit Thread subThread: Lock: 1 -> 0                              Unlock: 0 -> 1.
 */
void test_1EncapsulatedThread_2Locks() {
    threads[0]->thread = std::thread(help_Encapsulated_2Lock_Function, 0, 0);
    threads[0]->thread.join();
}

/**
 * Dies ist eine Hilfsmethode, welche verschiedene Lockreihenfolgen von je einem oder zwei Mutexe für den aufrufenden Thread bereithält.
 * @param caseNumber, ob nur ein oder zwei Mutexe gelockt werden sollen.
 * @param aqFirst, mid von Mutex, welches als erstes gelockt werden soll.
 * @param aqSecond,mid von Mutex, welches als zweites gelockt werden soll.
 * @param threadID, welcher Thread der ausführende Thread ist.
 */
void lockFunction(int caseNumber, int aqFirst, int aqSecond, int threadID) {
    switch (caseNumber) {
        case 0: // zwei Mutexe
            LG.acquire(threadID, aqFirst);
            LG.acquire(threadID, aqSecond);
            LG.release(threadID, aqSecond);
            LG.release(threadID, aqFirst);
            break;
        case 1: // ein Mutex
            LG.acquire(threadID, aqFirst);
            LG.release(threadID, aqFirst);
            break;
        default:
            break;
    }
}

/**
 * Dies ist eine Hilfsmethode, welche verschiedene Lockreihenfolgen von je drei Mutexe für den aufrufenden Thread bereitstellt.
 * @param aqFirst, mid von Mutex, welches als erstes gelockt werden soll.
 * @param aqSecond, mid von Mutex, welches als zweites gelockt werden soll.
 * @param aqThird,mid von Mutex, welches als drittes gelockt werden soll.
 * @param threadID, welcher Thread der ausführende Thread ist.
 */
void help_Function_3Locks(int aqFirst, int aqSecond, int aqThird, int threadID) {
    LG.acquire(threadID, aqFirst);
    LG.acquire(threadID, aqSecond);
    LG.acquire(threadID, aqThird);
    LG.release(threadID, aqThird);
    LG.release(threadID, aqSecond);
    LG.release(threadID, aqFirst);
}

/**
 * Dies ist eine Hilfsmethode, welche auf einen Thread hintereinander zwei verschiedene Lockreihenfolge mit zwei Mutexen ausführt.
 * Erster Aufruf:           Lock: 0 -> 1      Unlock: 1 -> 0.
 * Zweiter Aufruf:          Lock: 1 -> 0      Unlock: 0 -> 1.
 * @param threadID, welcher Thread der ausführende Thread ist.
 */
void help_OneThread_Function(int threadID) {
    lockFunction(0, 0, 1, threadID);
    lockFunction(0, 1, 0, threadID);
}

/**
 * Dies ist eine Hilfsmethode, welche den aufrufenden Thread erst schlafen legt und anschließend eine Lockreihenfolge mit zwei Mutexen ausführt.
 * Aufruf:                  Lock: 1 -> 0      Unlock: 0 -> 1.
 * @param caseNumber, ob nur ein oder zwei Mutexe gelockt werden sollen.
 * @param aqFirst, mid von Mutex, welches als erstes gelockt werden soll.
 * @param aqSecond, mid von Mutex, welches als zweites gelockt werden soll.
 * @param threadID, welcher Thread der ausführende Thread ist.
 */
void help_TimeShift_Function(int caseNumber, int aqFirst, int aqSecond, int threadID) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    lockFunction(caseNumber, aqFirst, aqSecond, threadID);
}

/**
 * Dies ist eine Hilfsmethode, welche auf den aufrufenden Thread das Mutex  mid1 locked, anschließend einen neuen Thread Subthread startet und danach Mutex mid entlocked.
 * Der Subthread lockt ein unterschiedliches Mutex als der Hauptthread.
 * Aufrufender Thread:      Lock: 0         Unlock: 0.
 * Aufruf mit subThread:    Lock: 1         Unlock: 1.
 * @param mid1, welches Mutex vom Hauptthread gelockt werden soll.
 * @param threadID, welcher Thread der ausführende Thread ist.
 */
void help_Encapsulated_Function(int mid1, int threadID) {
    LG.acquire(threadID, mid1);
    threads[1]->thread = std::thread(lockFunction, 1, 1, -1, 1);
    threads[1]->thread.join();
    LG.release(threadID, mid1);
}

/**
 * Dies ist eine Hilfsmethode, welche auf den aufrufenden Thread das Mutex mid1 locked, anschließend einen neuen Thread Subthread startet und mid1 entlocked.
 * Der Subthread versucht das gleiche Mutex, wie der Hauptthread zu locken.
 * Aufrufender Thread:      Lock: 0         Unlock: 0.
 * Aufruf mit subThread:    Lock: 0 -> 1      Unlock: 1 -> 0.
 * @param mid1, welches Mutex vom Hauptthread gelockt werden soll.
 * @param threadID welcher Thread der ausführende Thread ist.
 */
void help_Encapsulated_2Lock_Function(int mid1, int threadID) {
    LG.acquire(threadID, mid1);
    threads[1]->thread = std::thread(lockFunction, 0, 1, 0, 1);
    threads[1]->thread.join();
    LG.release(threadID, mid1);
}
