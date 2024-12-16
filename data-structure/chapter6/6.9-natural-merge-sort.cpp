/**
 * 时间复杂度：
 * 最优情况：当数组已经完全有序时，每次扫描只需要一次合并操作，
 * 时间复杂度为 O(n)。
 * 平均情况：在一般情况下，时间复杂度为 O(nlogn)。
 * 这是因为每次扫描都可能将运行的长度增加一倍。
 * 最坏情况：当数组完全逆序时，时间复杂度为 O(nlogn)。
 */

#include <iostream>
#include <vector>
using namespace std;

void merge(vector<int> &arr, int start, int mid, int end) {
  int n1 = mid - start;
  int n2 = end - mid;
  vector<int> left(arr.begin() + start, arr.begin() + mid);
  vector<int> right(arr.begin() + mid, arr.begin() + end);

  int i = 0, j = 0, k = start;
  while (i < n1 && j < n2) {
    if (left[i] <= right[j]) {
      arr[k++] = left[i++];
    } else {
      arr[k++] = right[j++];
    }
  }
  while (i < n1) {
    arr[k++] = left[i++];
  }
  while (j < n2) {
    arr[k++] = right[j++];
  }
}

void naturalMergeSort(vector<int> &arr) {
  int n = arr.size();
  bool isSorted = false;

  while (!isSorted) {
    isSorted = true;
    int start = 0;

    while (start < n) {
      // 找到第一个有序的子序列
      int mid = start + 1;
      while (mid < n && arr[mid - 1] <= arr[mid]) {
        mid++;
      }

      // 如果已经到达数组末尾，跳出
      if (mid >= n) {
        break;
      }

      // 找到下一个有序的子序列
      int end = mid + 1;
      while (end < n && arr[end - 1] <= arr[end]) {
        end++;
      }

      // 合并两个有序的运行
      merge(arr, start, mid, end);

      // 标记数组是否已经完全有序
      if (end < n) {
        isSorted = false;
      }

      start = end;
    }
  }
}

int main() {
  vector<int> arr = {5, 2, 4, 6, 1, 3, 2, 6};

  naturalMergeSort(arr);
  for (auto num : arr) {
    cout << num << " ";
  }
  cout << endl;

  return 0;
}