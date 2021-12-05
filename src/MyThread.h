#include <thread>

/**
 * Die Klasse MyThread stellt zu einem Thread eine Thread ID (mid) zur Verfügung.
 */
class MyThread {
public:
    int tid = -1;
    std::thread thread;

    /**
     * Erstellt ein Objekt MyThread.
     */
    MyThread() = default;

    /**
     * Übergibt einem Objekt MyThread eine tid.
     * @param mid Thread Id.
     */
    explicit MyThread(int tid) {
        this->tid = tid;
    }
};
