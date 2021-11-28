#include <map>
// in Set mehr als nur bool speichern, zb threadID, vorgänger, Uhrzeit
class Set {
public:
    std::map<int, bool> mySet = std::map<int, bool>();
    Set() = default;
    ~Set() = default;

    bool empty() {
        return mySet.empty();
    }

    bool elem(int n) {
        return mySet[n];
    }

    void add(int n) {
        mySet[n] = true;
    }

    void remove(int n){
        mySet[n] = false;
    }

    static std::tuple<Set, bool> unionSets(Set a, Set b) {
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