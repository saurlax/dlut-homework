/**
 * 应用栈结构，编码实现数制转换
 */

#include <iostream>
#include <stack>
using namespace std;

void conversion(int n, int base) {
  stack<int> s;
  while (n) {
    s.push(n % base);
    n /= base;
  }
  while (!s.empty()) {
    int x = s.top();
    printf("%c", x < 10 ? x + '0' : x - 10 + 'A');
    s.pop();
  }
}

int main() {
  int n, base;
  cin >> n >> base;
  conversion(n, base);
  return 0;
}