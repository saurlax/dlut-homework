/**
 * 分别用穷举法和迭代法计算两个整数的最大公约数。
 */

#include <iostream>
using namespace std;

int exhaustion(int a, int b) {
  int min = a < b ? a : b;
  for (int i = min; i > 0; i--) {
    if (a % i == 0 && b % i == 0) {
      return i;
    }
  }
  return 1;
}

int iteration(int a, int b) {
  while (b != 0) {
    int temp = a % b;
    a = b;
    b = temp;
  }
  return a;
}

int main() {
  int a, b;
  cin >> a >> b;
  cout << "exhaustion: " << exhaustion(a, b) << endl;
  cout << "iteration: " << iteration(a, b) << endl;
  return 0;
}