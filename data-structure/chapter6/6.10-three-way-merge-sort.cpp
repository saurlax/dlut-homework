/**
 * 时间复杂度：O(nlogn)
 * 空间复杂度：O(n)
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

void threeWayMerge(vector<int> &arr, int start, int firstMid, int secondMid,
                   int end) {
  int n1 = firstMid - start;
  int n2 = secondMid - firstMid;
  int n3 = end - secondMid;

  vector<int> left(arr.begin() + start, arr.begin() + firstMid);
  vector<int> middle(arr.begin() + firstMid, arr.begin() + secondMid);
  vector<int> right(arr.begin() + secondMid, arr.begin() + end);

  int i = 0, j = 0, k = 0;
  int idx = start;

  // 合并三个子数组
  while (i < n1 && j < n2 && k < n3) {
    if (left[i] <= middle[j] && left[i] <= right[k]) {
      arr[idx++] = left[i++];
    } else if (middle[j] <= left[i] && middle[j] <= right[k]) {
      arr[idx++] = middle[j++];
    } else {
      arr[idx++] = right[k++];
    }
  }

  // 合并剩余的两个子数组
  while (i < n1 && j < n2) {
    if (left[i] <= middle[j]) {
      arr[idx++] = left[i++];
    } else {
      arr[idx++] = middle[j++];
    }
  }

  while (j < n2 && k < n3) {
    if (middle[j] <= right[k]) {
      arr[idx++] = middle[j++];
    } else {
      arr[idx++] = right[k++];
    }
  }

  while (i < n1 && k < n3) {
    if (left[i] <= right[k]) {
      arr[idx++] = left[i++];
    } else {
      arr[idx++] = right[k++];
    }
  }

  // 合并剩余的一个子数组
  while (i < n1) {
    arr[idx++] = left[i++];
  }
  while (j < n2) {
    arr[idx++] = middle[j++];
  }
  while (k < n3) {
    arr[idx++] = right[k++];
  }
}

void threeWayMergeSort(vector<int> &arr, int start, int end) {
  if (end - start <= 1) {
    return;
  }

  int size = end - start;
  int firstMid = start + size / 3;
  int secondMid = start + 2 * size / 3;

  threeWayMergeSort(arr, start, firstMid);
  threeWayMergeSort(arr, firstMid, secondMid);
  threeWayMergeSort(arr, secondMid, end);

  threeWayMerge(arr, start, firstMid, secondMid, end);
}

int main() {
  vector<int> arr = {38, 27, 43, 3, 9, 82, 10, 15, 27, 43, -5, 0, 99, 27};

  threeWayMergeSort(arr, 0, arr.size());

  for (auto num : arr) {
    cout << num << " ";
  }
  cout << endl;

  return 0;
}