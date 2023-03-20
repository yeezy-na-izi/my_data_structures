#include "vector/Vector.h"
#include <iostream>

using namespace std;


int main() {
    Vector v1(nullptr, 0, 2);
    cout << v1.size() << " " << v1.capacity() << " " << v1.loadFactor() << endl;
    v1.pushBack(10);
    cout << v1.size() << " " << v1.capacity() << " " << v1.loadFactor() << endl;
    v1.pushFront(11);
    for (Value i: v1) {
        cout << i << " ";
    }
    cout << endl;
    v1.shrinkToFit();

    cout << v1.size() << " " << v1.capacity() << " " << v1.loadFactor() << endl;

}