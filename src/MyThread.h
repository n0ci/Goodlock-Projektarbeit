#include <thread>


class MyThread{
public:
    MyThread() = default;

    int tid = -1;
    std::thread thread;

    explicit MyThread(int tid){
        this->tid = tid;
    }
};
