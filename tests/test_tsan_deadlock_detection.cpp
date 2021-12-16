#include <pthread.h>
#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

// Tests
void test_simple_pthread_example();

void test_ClassicDeadlock();

void test_TimeShiftDeadlock();

void test_OnlyOneThread();

void test_ThreeThreads_OneCycle();

void test_ThreeThreads_MoreCycles();

void test_TwoThreads_3_2Locks();

void test_TwoThreads_3Locks();

void test_1Encapsulated_1Threads();

void test_1Encapsulated_2Threads();

void test_1EncapsulatedThread_2Locks();


//Hilfsmethoden
void help_Function_1Lock(int);

void help_Function_2Locks(int);

void help_OneThread_Function();

void help_Function_3Locks(int);

void help_Encapsulated_Function();

void help_Encapsulated_2Lock_Function();

mutex x, y, z;

/**
 * Die Main Methode ist der Eintrittspunkt des Programms
 * @return default Wert 0
 */
int main() {
    //test_simple_pthread_example();
    test_ClassicDeadlock();
    //test_TimeShiftDeadlock();
    //test_OnlyOneThread();
    //test_ThreeThreads();
    //test_ThreeThreads_MoreCycles();
    //test_TwoThreads_3_2Locks();
    //test_TwoThreads_3Locks();
    //test_1Encapsulated_1Threads();
    //test_1Encapsulated_2Threads();
    //test_1EncapsulatedThread_2Locks();
    return 0;
}

/**
 * Ein Test, der mit einem pthread und zwei Mutex(px, py) arbeitet.
 * Der potenzielle Deadlock entsteht, da einmal zuerst px und dann py und
 * beim anderen zuerst py und dann px gelocked werden.
 * Jedoch findet das alles auf einem pthread statt, weshalb kein Deadlock entstehen kann.
 * false positive
 */
void test_simple_pthread_example() {
    pthread_mutex_t px, py;
    pthread_mutex_init(&px, nullptr);
    pthread_mutex_init(&py, nullptr);

    pthread_mutex_lock(&px);
    pthread_mutex_lock(&py);
    pthread_mutex_unlock(&py);
    pthread_mutex_unlock(&px);

    pthread_mutex_lock(&py);
    pthread_mutex_lock(&px);
    pthread_mutex_unlock(&px);
    pthread_mutex_unlock(&py);

    pthread_mutex_destroy(&px);
    pthread_mutex_destroy(&py);
}

/**
 * Dies ist eine Hilfsmethode, welche verschiedene Lockreihenfolge von je zwei Mutexen für den aufrufenden Thread bereithält.
 * @param number bestimmt, welcher Fall durchgeführt werden soll.
 */
void help_Function_2Locks(int number) {
    switch (number) {
        case 0:
            x.lock();
            y.lock();
            y.unlock();
            x.unlock();
            break;
        case 1:
            y.lock();
            x.lock();
            x.unlock();
            y.unlock();
            break;
        case 2:
            z.lock();
            x.lock();
            x.unlock();
            z.unlock();
            break;
        case 3:
            x.lock();
            z.lock();
            z.unlock();
            x.unlock();
            break;
        default:
            break;
    }
}

/**
 * Dies ist eine Hilfsmethode, welche verschiedene Lockreihenfolge von je drei Mutexen für den aufrufenden Thread bereithält.
 * @param number bestimmt, welcher Fall durchgeführt werden soll.
 */
void help_Function_3Locks(int number) {
    switch (number) {
        case 0:
            x.lock();
            y.lock();
            z.lock();
            z.unlock();
            y.unlock();
            x.unlock();
            break;
        case 1:
            x.lock();
            z.lock();
            y.lock();
            y.unlock();
            z.unlock();
            x.unlock();
            break;
        default:
            break;
    }
}

/**
 * Dies ist eine Hilfsmethode, welche verschiedene Lockreihenfolge von je einem Mutex für den aufrufenden Thread bereithält.
 * @param number bestimmt, welche Fall durchgeführt werden soll.
 */
void help_Function_1Lock(int number) {
    switch (number) {
        case 0:
            x.lock();
            y.unlock();
        case 1:
            y.lock();
            y.unlock();
        case 2:
            z.lock();
            z.unlock();
        default:
            break;
    }
}

/**
 * Dies ist eine Hilfsmethode, welche auf den aufrufenden Thread direkt hintereinander zwei verschiedene Lockreihenfolge ausführt.
 * Aufruf mit 0: Lock: x -> y       Unlock: y -> x
 * Aufruf mit 1: Lock: y -> x       Unlock: x -> y
 */
void help_OneThread_Function() {
    help_Function_2Locks(0);
    help_Function_2Locks(1);
}

/**
 * Dies ist eine Hilfsmethode, welche den aufrufenden Thread erst schlafen legt und anschließend eine Lockreihenfolge ausführt.
 * Aufruf mit 1: Lock: y -> x       Unlock: x -> y
 */
void help_TimeShift_Function() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    help_Function_2Locks(1);
}

/**
 * Dies ist eine Hilfsmethode, welche auf den aufrufenden Thread Mutex x locked, anschließend einen neuen Thread Subthread startet und Mutex x entlocked.
 * Der Subthread lockt ein unterschiedliches Mutex als der Hauptthread.
 * Aufruf mit subThread: Lock: y    Unlock: y
 */
void help_Encapsulated_Function() {
    printf("in helpFunction von t1\n");
    x.lock();
    std::thread subThread(help_Function_1Lock, 1);
    printf("vor subaufruf\n");
    subThread.join();
    printf("nach subaufruf\n");
    x.unlock();
}
/**
 * Dies ist eine Hilfsmethode, welche auf den aufrufenden Thread Mutex x locked, anschließend einen neuen Thread Subthread startet und Mutex x entlocked.
 * Der Subthread versucht das gleiche Mutex, wie der Hauptthread zu locken.
 * Aufruf mit subThread: Lock: y -> x   Unlock: x -> y
 */
void help_Encapsulated_2Lock_Function(){
    x.lock();
    std::thread subThread(help_Function_2Locks,1);
    subThread.join();
    x.unlock();
}

/**
 * Diese Testmethode testet einen klassischen Deadlock.
 * Er erkennt einen potenziellen Deadlock.
 * true positive
 * Aufruf mit Thread t0: Lock: x -> y     Unlock: y -> x
 * Aufruf mit Thread t1: Lock: y -> x     Unlock: x -> y
 */
void test_ClassicDeadlock() {
    std::thread t0(help_Function_2Locks, 0);
    std::thread t1(help_Function_2Locks, 1);
    t0.join();
    t1.join();
}

/**
 * Diese Testmethode testet, ob der TSan zwei zeitversetzte Lockreihenfolgen auf zwei Threads trotzdem als potenziellen Deadlock wahrnimmt.
 * Obwohl das nie zu einem Deadlock führen kann, da t1 erst anfängt, wenn t0 schon lange fertig ist.
 * false positive
 * Aufruf mit Thread t0: Lock: x -> y     Unlock: y -> x
 * t1 schläft
 * Aufruf mit Thread t1: Lock: y -> x     Unlock: x -> y
 *
 */
void test_TimeShiftDeadlock() {
    std::thread t0(help_Function_2Locks, 0);
    std::thread t1(help_TimeShift_Function);
    t0.join();
    t1.join();
}

/**
 * Diese Testmethode testet, ob der TSan zwei Lockreihenfolgen auf einem Thread als potenziellen Deadlock erkennt.
 * Dieser eine Thread kann keinen Deadlock produzieren.
 * false positive
 * Aufruf mit Thread t: Lock: x -> y      Unlock: y -> x
 * Aufruf mit Thread t: Lock: y -> x      Unlock: x -> y
 */
void test_OnlyOneThread() {
    std::thread t(help_OneThread_Function);
    t.join();
}

/**
 * Diese Testmethode testet, ob der TSan mit mehr als nur zwei Threads einen Zyklus erkennt.
 * Er erkennt einen potenziellen Deadlock bei dem Mutex x und Mutex y.
 * true positive
 * Aufruf mit Thread t0: Lock: x -> y     Unlock: y -> x
 * Aufruf mit Thread t1: Lock: y -> x     Unlock: x -> y
 * Aufruf mit Thread t2: Lock: z -> x     Unlock: z -> x
 */
void test_ThreeThreads_OneCycle() {
    std::thread t0(help_Function_2Locks, 0);
    std::thread t1(help_Function_2Locks, 1);
    std::thread t2(help_Function_2Locks, 2);
    t0.join();
    t1.join();
    t2.join();
}
/**
 * Diese Testmethode testet, ob der TSan mit mehr als nur zwei Threads mehrere Zyklen erkennt.
 * Er erkennt zwei potenzielle Deadlock bei t0 und t1 und bei t2 und t3.
 * true positive
 * Aufruf mit Thread t0: Lock: x -> y     Unlock: y -> x
 * Aufruf mit Thread t1: Lock: y -> x     Unlock: x -> y
 * Aufruf mit Thread t2: Lock: z - >x     Unlock: x -> z
 * Aufruf mit Thread t3: Lock: x -> z     Unlock: z -> x
 */
void test_ThreeThreads_MoreCycles() {
    std::thread t0(help_Function_2Locks, 0);
    std::thread t1(help_Function_2Locks, 1);
    std::thread t2(help_Function_2Locks, 2);
    std::thread t3(help_Function_2Locks, 3);
    t0.join();
    t1.join();
    t2.join();
    t3.join();
}

/**
 * Diese Testmethode testet, ob der TSan mit zwei Treads und unterschiedlichen Anzahl von locks den Deadlock erkennt.
 * Er erkennt diesen potenziellen Deadlock.
 * true positive
 * Aufruf mit Thread t0: Lock: x -> y -> z   Unlock: z -> y -> x
 * Aufruf mit Thread t1: Lock: y -> x        Unlock: x -> y

 */
void test_TwoThreads_3_2Locks() {
    std::thread t0(help_Function_3Locks, 0);
    std::thread t1(help_Function_2Locks, 1);
    t0.join();
    t1.join();
}

/**
 * Diese Testmethode testet, ob der TSan mit zwei Threads aber gleichem Startlock einen potenziellen Deadlock erkennt.
 * Er erkennt einen potenziellen Deadlock wegen t0: y -> z und t1: z -> y.
 * Jedoch kann das nie zu einem Deadlock führen, da bevor diese Mutexe gelocked werden können, muss das Mutex x gelocked sein.
 * Und dieses kann immer nur von einem Thread gelocked sein und der andere Thread muss warten.
 * false positive
 * Aufruf mit Thread t0: Lock: x -> y -> z      Unlock: z -> y -> x
 * Aufruf mit Thread t1: Lock: x -> z -> y      Unlock: y -> z -> x
 */
void test_TwoThreads_3Locks() {
    std::thread t0(help_Function_3Locks, 0);
    std::thread t1(help_Function_3Locks, 1);
    t0.join();
    t1.join();
}

/**
 * Diese Testmethode testet, ob der TSan einen verschachtelten Thread mit jeweils einem Lock erkennt und darin kein Deadlock erkennt.
 * true negative, jedoch erkennt er die Abhängigkeit der Threads nicht.
 * Aufruf mit Thread t: Lock: Mutex x und starte Thread subThread   Unlock: x
 * Aufruf mit Thread subThread: Lock: y                             Unlock: y
 */
void test_1Encapsulated_1Threads() {
    std::thread t(help_Encapsulated_Function);
    t.join();
}

/**
 * Diese Testmethode testet, ob der TSan einen Deadlock bei einem verschachtelten und einem normalen Thread mit verschiedener Lockreihenfolge erkennt.
 * Erkennt die Verschachtelung und somit die Abhängigkeit der Threads nicht.
 * false negative
 * Aufruf mit Thread t1: Lock: x und starte Thread subThread        Unlock: x
 * Aufruf mit Thread subThread: Lock y                              Unlock: y
 * Aufruf mit Thread t2: Lock: y -> x                               Unlock: x -> y
 */
void test_1Encapsulated_2Threads() {
    std::thread t1(help_Encapsulated_Function);
    std::thread t2(help_Function_2Locks, 1);
    printf("starte t1\n");
    t1.join();
    printf("starte t2\n");
    t2.join();
    printf("nach t2 \n");
}

/**
 * Diese Testmethode testet, ob der TSan einen Deadlock bei einem verschachtelten Thread mit verschiedener Anzahl an Lockreihenfolgen erkennt.
 * Es wird ein Deadlock erzeugt und so kann TSan diesen nicht erkennen, da dass Programm sich aufhängt.
 * verursacht Deadlock - keine Aussage möglich
 * Aufruf mit Thread t1: Lock: x und starte Thread subThread        Unlock: x
 * Aufruf mit Thread subThread:  Lock: y -> x                       Unlock: x -> y
 */
void test_1EncapsulatedThread_2Locks(){
    std::thread t(help_Encapsulated_2Lock_Function);
    t.join();
}


