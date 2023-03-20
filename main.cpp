#include "vector/Vector.h"
#include <iostream>

using namespace std;


int main() {
    int *a = new int[10];
    for (int i = 0; i < 10; ++i) {
        a[i] = i;
    }
    Vector v1(a, 10, 2);
    cout << "v1: ";
    cout << v1.size() << " " << v1.capacity() << " " << v1.loadFactor() << endl;
}