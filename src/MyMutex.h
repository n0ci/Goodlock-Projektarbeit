#include <mutex>

class MyMutex{
    public:
    MyMutex() = default;

    int mid = -1;
    std::mutex mutex;

    explicit MyMutex(int mid){
        this->mid = mid;
    }
};
