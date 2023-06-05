#include <iostream>
#include <vector>


int f(int x) {
    return (x + 1) % 10;
}


std::pair<int, int> floyd(int x0) {
    int tortoise = f(x0);
    int hare = f(f(x0));
    while (tortoise != hare) {
        tortoise = f(tortoise);
        hare = f(f(hare));
    }
    int mu = 0;
    tortoise = x0;
    while (tortoise != hare) {
        tortoise = f(tortoise);
        hare = f(hare);
        mu += 1;
    }
    int lam = 1;
    hare = f(tortoise);
    while (tortoise != hare) {
        hare = f(hare);
        lam += 1;
    }
    return std::make_pair(lam, mu);
}


int main() {
    int x0 = 2;
    std::pair<int, int> ans = floyd(x0);
    std::cout << ans.first << " " << ans.second;
    return 0;
}