/**
 * 请完成栈类。实现弹栈、压栈，以及重载[]运算符。
 */

#include <iostream>
using namespace std;

const int MAX = 10;

class Stack {
  double st[MAX];
  int top;

 public:
  Stack() : top(-1) {}
  void push(const double& number) {
    if (top < MAX - 1) st[++top] = number;
  }
  double pop() {
    if (top >= 0) return st[top--];
    return -1;
  }
  double& operator[](int index) { return st[index]; }
};

int main() {
  Stack st1;
  st1.push(11.1);
  st1.push(22.2);
  st1.push(33.3);
  cout << st1.pop() << endl;
  st1[1] = 0;
  cout << st1[0] << "," << st1[1] << endl;
  return 0;
}