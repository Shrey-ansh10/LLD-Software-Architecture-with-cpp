/*
Q4 — Compile-time polymorphism

Part A: free functions named maxOf
- maxOf(int, int)
- maxOf(double, double)
- maxOf(const string&, const string&) — later in dictionary order

Return type alone must not be how you distinguish them.

Part B: class Point with int x, y
- constructor (int x, int y) (use this-> if names clash)
- Point operator+(const Point&) const
- void print() const

main: print maxOf(3, 9), maxOf(2.5, 2.7), maxOf("cat", "dog"), and (1,2) + (3,4) → 4 6.

Done when: you can say these overloads are chosen before the program runs.
*/

#include <iostream>
using namespace std;

int maxOf(int a, int b){
    return max(a,b);
}

double maxOf(double a, double b){
    return max(a,b);
}

string maxOf(const string& a, const string& b){
    return max(a, b);
}


class Point{
    int x;
    int y;

public:
    Point() : x(0), y(0){
    }

    Point(int a, int b): x(a), y(b){
        cout << "points created" << endl;
    }

    Point operator+(const Point& a) const{
        Point ans; // create new point object - this will take the value of object attributes after the operator

        ans.x = x + a.x; // adds x of both objects
        ans.y = y + a.y; // adds y of both objects

        return ans; // return the object
    }

    void display(){
        cout << "x : " << x << "\ny : " << y << endl;
    }

};


int main(){

    cout<< maxOf(3, 9) << endl;
    cout << maxOf(2.5, 2.7) << endl;
    cout << maxOf("cat", "dog") << endl;

    Point p1(1,2);
    Point p2(3,4);

    Point p3 = p1+p2;
    p3.display();

}
