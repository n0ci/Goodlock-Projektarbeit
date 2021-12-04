#include <mutex>

/**
 * Die Klasse MyMutex stellt zu einem Mutex eine Mutex ID (mid) zur Verfügung.
 */
class MyMutex{
public:
    int mid = -1;
    std::mutex mutex;

    /**
     * Erstellt ein Objekt MyMutex.
     */
    MyMutex()= default;

    /**
     * Übergibt einem Objekt MyMutex eine mid.
     * @param mid Mutex Id.
     */
    explicit MyMutex(int mid){
        this->mid = mid;
    }
};
