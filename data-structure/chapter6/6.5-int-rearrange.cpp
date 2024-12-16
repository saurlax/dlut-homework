/**
 * 设计并实现一个有效的对n个整数重排的算法，使得所有的负数位于非负数之前，给出算法的性能分析
 *
 * 时间复杂度：该算法只需遍历数组一次，时间复杂度为 O(n)，其中 n 是数组的长度。
 * 空间复杂度：该算法在原地对数组进行操作，除了使用额外的指针变量外，
 * 不需要额外的存储空间，空间复杂度为 O(1)。
 */

#include <iostream>
#include <vector>
using namespace std;

void rearrange(vector<int> &arr) {
  int left = 0;
  int right = arr.size() - 1;

  while (left < right) {
    // 左指针右移，直到遇到非负数
    while (left < right && arr[left] < 0)
      left++;
    // 右指针左移，直到遇到负数
    while (left < right && arr[right] >= 0)
      right--;
    if (left < right) {
      swap(arr[left], arr[right]);
      left++;
      right--;
    }
  }
}

int main() {
  vector<int> arr = {-1, 3, -5, 6, -2, 4, -7, 0, 8};
  rearrange(arr);

  for (int num : arr)
    cout << num << " ";
  cout << endl;

  return 0;
}