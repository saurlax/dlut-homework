/**
 * 用new和delete运算符实现动态内存空间管理的方法编写程序。
 * 从键盘输入3*3整型数组的数据，将数据存储在动态内存空间，计算并输出所有元素之和、最大值和最小值。
 * 提示：申请空间可以采用两种方法中任意一种：
 * （1）一次申请二维数组；
 * （2）对二维数组一行一行申请。
 */

#include <iostream>
using namespace std;

int main() {
  int **arr = new int *[3];
  for (int i = 0; i < 3; i++) {
    arr[i] = new int[3];
  }

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      cin >> arr[i][j];
    }
  }

  int sum = 0, max = arr[0][0], min = arr[0][0];
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      sum += arr[i][j];
      if (arr[i][j] > max) max = arr[i][j];
      if (arr[i][j] < min) min = arr[i][j];
    }
  }
  cout << "sum: " << sum << endl;
  cout << "max: " << max << endl;
  cout << "min: " << min << endl;
  return 0;
}