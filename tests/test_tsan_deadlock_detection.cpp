#include <mutex>
#include <atomic>
#include <iostream>
#include <thread>
#include <unistd.h>
using namespace std;
void helpSubFunctionC();

std::mutex x;
std::mutex y;

void helpFunctionA(){
    x.lock();
    y.lock();
    y.unlock();
    x.unlock();

}

void helpFunctionB(){
    y.lock();
    x.lock();
    x.unlock();
    y.unlock();
}

void helpOneThread(){
    y.lock();
    x.lock();
    x.unlock();
    y.unlock();
    y.lock();
    x.lock();
    x.unlock();
    y.unlock();
}

void helpFunctionC() {      //Macht einen nicht detecten Deadlock
    printf("sind in helpFunction von t1\n");
    x.lock();
    std::thread subThread(helpSubFunctionC);
    printf("sind vor subaufruf\n");
    subThread.join();
    printf("sind nach subaufruf\n");
    x.unlock();
}

void helpSubFunctionC() {
    printf("sind in helpSubFunctionC\n");
    y.lock();
    y.unlock();
    printf("sind am ende der subFunction\n");
}

void testDeadlock(){
    std::thread t1(helpFunctionA);
    std::thread t2(helpFunctionB);
    t1.join();
    t2.join();
}

void testShiftDeadlock(){           // Zeit verzögerte Threads bilden kein Zyklus (kein false positive)
    std::thread t1(helpFunctionA);
    std::thread t2(helpFunctionB);
    t1.join();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    t2.join();
}

void testOneThread(){               //hier bei TSan kein false positive
    std::thread t(helpOneThread);
    t.join();
}

void testIndirectDeadLock(){
    std::thread t1(helpFunctionC);
    std::thread t2(helpFunctionB);
    printf("starte t1\n");
    t1.join();
    printf("starte t2\n");
   // t2.join();
}

int main() {
    //testDeadlock();
    //testShiftDeadlock();
    //testOneThread();
   printf("vor start\n");
    testIndirectDeadLock();
}
