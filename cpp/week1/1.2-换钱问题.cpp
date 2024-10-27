/**
 * 将1元人民币兑换成1、2、5分的人民币，有多少种换法？
 */

#include <iostream>
using namespace std;

int main() {
  int count = 0;
  for (int a = 0; a * 0.01 <= 1; a++)
    for (int b = 0; b * 0.02 <= 1; b++)
      for (int c = 0; c * 0.05 <= 1; c++)
        if (a * 0.01 + b * 0.02 + c * 0.05 == 1) count++;
  cout << count << endl;
  return 0;
}