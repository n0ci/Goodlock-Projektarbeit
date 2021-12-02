#include <map>
// in MySet mehr als nur bool speichern, zb threadID, vorgänger, Uhrzeit
class MySet {
public:
    std::map<int, bool> mySet = std::map<int, bool>();
    MySet() = default;
    ~MySet() = default;

    bool empty() {
        return mySet.empty();
    }

    bool elem(int n) {
        return mySet.at(n);
    }

    void add(int n) {
        mySet.at(n) = true;
    }

    void add_unsafe(int n) {
        mySet[n] = true;
    }

    void remove(int n){
        mySet.at(n) = false;
    }

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