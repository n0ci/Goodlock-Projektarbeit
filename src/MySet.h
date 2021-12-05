#include <map>

/**
 * Die Klasse MySet enthält eine Map <int, bool> und funktionen, welche auf dieser Map arbeiten.
 * Im Wesentlichen repräsentiert die Map, ob ein Mutex ein gelocked ist, oder nicht.
 */
class MySet {
public:
    std::map<int, bool> mySet = std::map<int, bool>();

    /**
     * Default constructor.
     */
    MySet() = default;

    /**
     * Default destructor.
     */
    ~MySet() = default;

    /**
     * Überprüft, ob die Map mySet leer ist.
     * @return True wenn leer.
     */
    bool empty() {
        return mySet.empty();
    }

    /**
     * Gibt die Value für einen Key (MID) in mySet zurück.
     * @param mid Die ID des Mutex.
     * @return Wahrheitswert, sagt aus, ob das Mutex mit MID erworben wurde, oder nicht.
     */
    bool elem(int mid) {
        return mySet.at(mid);
    }

    /**
     * Gibt die Value für einen Key (MID) in mySet zurück. Falls Key nicht in Map mySet, wird ein K,V pair erstellt
     * und eingefügt. In diesem Fall ist der return Wert standardmäßig false.
     * Sollte nur in Sonderfällen verwendet werden.
     * @param mid Die ID des Mutex.
     * @return Wahrheitswert, sagt aus, ob das Mutex mit MID erworben wurde, oder nicht.
     */
    bool elem_unsafe(int mid) {
        return mySet[mid];
    }

    /**
     * Ändert die Value für einen Key (MID) in mySet auf true.
     * @param mid Die ID des Mutex.
     */
    void add(int mid) {
        mySet.at(mid) = true;
    }

    /**
     * Ändert die Value für einen Key (MID) in mySet auf true. Falls Key nicht in Map mySet, wird ein K,V pair erstellt
     * und eingefügt. In diesem Fall ist der return Wert standardmäßig false.
     * @param mid Die ID des Mutex.
     */
    void add_unsafe(int mid) {
        mySet[mid] = true;
    }

    /**
     * Ändert die Value für einen Key (MID) in mySet auf false.
     * @param mid Die ID des Mutex.
     */
    void remove(int mid) {
        mySet.at(mid) = false;
    }

    /**
     * Bekommt zwei MySet Objekte a und b und vereinigt Sie in a.
     * @param a Objekt vom Typ MySet, hier steht später die vereinigte Menge.
     * @param b Objekt vom Typ MySet, dieses wird bei Mengenungleichheit in a vereinigt.
     * @return Ein Tupel mit (MySet, Wahrheitswert).
     * Wobei der MySet die Vereinigung beider Objekte a und b ist.
     * Bei Mengengleichheit ist der Wahrheitswert true.
     */
    static std::tuple<MySet, bool> unionSets(MySet a, MySet b) {
        auto r = true;
        for (auto const&[key, val]: b.mySet) {
            if (!a.mySet.count(key)) {
                r = false;
                a.mySet.insert({key, val});
            }
        }
        return std::make_tuple(a, r);
    }
};