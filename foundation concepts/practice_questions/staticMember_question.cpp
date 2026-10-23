/* 

    Q7 — Static members
    Build: Session with static inline int live = 0

    constructor: live++
    destructor: live--
    static int count() — returns live
    count() must not use any non-static data member

    -> main must print:
        count before any object → 0
        create a → 1
        inner scope: create b → 2
        after inner scope → 1

    Done when: you see one shared counter, not one per object, and that it exists with zero objects.

*/

#include <iostream>
using namespace std;

class Session{
    static inline int live = 0;

public:
    Session(){
        live++;
    }

    static int count(){
        return live;
    }

    ~Session(){
        live--;
    }

};

int main(){

    cout << "No object created yet. -> Live = " << Session::count() << endl; // 0

    Session a;
    cout << "After fist object creation -> Live = " << Session::count() << endl; //1

    {
        Session b;
        cout << "After second object creation -> Live = " << Session::count() << endl; //2
    } // as the scope ends, b is destroyed, destructor reduce live
    
    cout << "After second object is destroyed -> Live = " << Session::count() << endl; // 1
    
    return 0;
}