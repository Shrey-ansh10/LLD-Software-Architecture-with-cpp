#include <iostream>
#include <string>
using namespace std;

// Compile-time / static polymorphism:
// The compiler picks WHICH function to call using the types at the call site.
// Decision is done before the program runs. No vtable.

class Complex {
    int real, img;

    public:
        // Constructor overloading is also compile-time polymorphism.
        Complex() : real(0), img(0) {}

        Complex(int real, int img) {
            // parameters shadow the members, so this-> is required here
            this->real = real;
            this->img = img;
        }

        void display() const {
            cout << real << " + " << img << "i" << endl;
        }

        // Operator overloading: same operator +, different meaning for this type.
        Complex operator+(const Complex& c) const {
            Complex ans;
            // another Complex may touch private members of this class
            ans.real = real + c.real;
            ans.img = img + c.img;
            return ans;
        }
    };

    // Function overloading: same name, different parameter lists.
    // Return type alone does NOT distinguish overloads.
    int add(int a, int b) {
        return a + b;
    }

    string add(const string& a, const string& b) {
        return a + " " + b;
    }

    int add(int a) {
        return a + a;
    }

    // int add(int c, int d) { }        // NOT ALLOWED — same signature as add(int, int)
    // string add(int a, int b) { }     // NOT ALLOWED — only return type differs

    // Templates: still compile-time. The compiler generates a version per type used.
    template <typename T>
    T bigger(T a, T b) {
        return (a > b) ? a : b;
}

int main() {
    cout << add(3, 3) << endl;
    cout << add("string a", "string b") << endl;
    cout << add(5) << endl;

    Complex c1(3, 4);
    Complex c2(5, 6);
    Complex c3 = c1 + c2; // resolved at compile time to Complex::operator+
    c3.display();

    cout << bigger(3, 9) << endl;
    cout << bigger(2.5, 1.1) << endl;

    return 0;
}
