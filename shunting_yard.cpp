#include <iostream>
#include <string>
#include <stack>
#include <map>
#include <cmath>

using namespace std;


void change(stack<char> &op, stack<int> &num) {
    if (op.top() == '(') {
        return;
    }
    if (num.size() == 1 and op.top() == '-') {
        num.top() *= -1;
        op.pop();
        return;
    }

    if (op.size() >= 2 and op.top() == '-' and op[op.size() - 2] == '-') {
        num.top() *= -1;
        op.pop();
        return;
    }

    int b = num.top();
    num.pop();
    int a = num.top();
    num.pop();
    if (op.top() == '+') {
        num.push(a + b);
    } else if (op.top() == '-') {
        num.push(a - b);
    } else if (op.top() == '*') {
        num.push(a * b);
    } else if (op.top() == '/') {
        num.push(a / b);
    } else if (op.top() == '^') {
        num.push(static_cast<int>(pow(a, b)));
    }
    op.pop();
}

long long shunting_yard(const string &s) {
    map<char, int> priority;
    priority['+'] = 3;
    priority['-'] = 3;
    priority['*'] = 2;
    priority['/'] = 2;
    priority['^'] = 1;
    priority['('] = 0;
    priority[')'] = 0;

    stack<char> op;
    stack<int> num;

    string cur_num;

    for (char i: s) {
        if (i >= '0' && i <= '9') {
            cur_num += i;
        } else {
            if (!cur_num.empty()) {
                num.push(stoi(cur_num));
                cur_num = "";
            }

            if (i == ')') {
                while (op.top() != '(') {
                    change(op, num);
                }
                op.pop();
            } else {
                while ((!op.empty() && priority[op.top()] <= priority[i]) && op.top() != '(') {
                    change(op, num);
                }
                op.push(i);
            }
        }
    }
    if (!cur_num.empty()) {
        num.push(stoi(cur_num));
    }

    while (!op.empty()) {
        change(op, num);
    }

    return num.top();
}

int main() {
    string s;
    getline(cin, s);
    cout << shunting_yard(s);
}