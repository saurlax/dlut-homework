/**
 * 输入一个整数n(大于0，默认用户输入合法，无需程序验证)，利用new申请长度为n的int型数组，
 * 输入n个整数到该数组，数组元素有正数有负数，数组中连续的一个或多个整数构成一个子数组。
 * 求所有子数组中“元素之和”最大的子数组（如有多个这样的子数组，仅输出一个）。
 */

#include <iostream>
using namespace std;

int main() {
  int n;
  cin >> n;
  int* nums = new int[n];
  for (int i = 0; i < n; i++) cin >> nums[i];

  int start = 0, end = 0, res = nums[0], sum = 0;
  for (int i = 0; i < n; i++) {
    if (sum < 0) {
      sum = nums[i];
      start = i;
    } else {
      sum += nums[i];
    }
    if (sum > res) {
      res = sum;
      end = i;
    }
  }

  for (int i = start; i <= end; i++) cout << nums[i] << " ";
  delete[] nums;
  return 0;
}