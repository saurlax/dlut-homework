/**
 * 实现顺序查找、二分查找及分块查找三种算法。
 */

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

int sequentialSearch(const vector<int> &arr, int key) {
  for (int i = 0; i < arr.size(); i++) {
    if (arr[i] == key) {
      return i;
    }
  }
  return -1;
}

int binarySearch(const vector<int> &arr, int key) {
  int left = 0, right = arr.size() - 1;
  while (left <= right) {
    int mid = left + (right - left) / 2;
    if (arr[mid] == key) {
      return mid;
    } else if (arr[mid] < key) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }
  return -1;
}

int blockSearch(const vector<int> &arr, int key, int blockSize = 3) {
  int n = arr.size();
  int numBlocks = ceil((double)n / blockSize);
  vector<int> blockMax(numBlocks, INT_MIN);

  for (int i = 0; i < n; i++) {
    int blockIndex = i / blockSize;
    blockMax[blockIndex] = max(blockMax[blockIndex], arr[i]);
  }

  auto it = lower_bound(blockMax.begin(), blockMax.end(), key);
  if (it == blockMax.end()) {
    return -1;
  }
  int blockIndex = distance(blockMax.begin(), it);

  int start = blockIndex * blockSize;
  int end = min(start + blockSize, n);
  for (int i = start; i < end; i++) {
    if (arr[i] == key) {
      return i;
    }
  }

  return -1;
}

int main() {
  vector<int> arr = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};

  int key = 7;
  cout << "Sequential Search: " << sequentialSearch(arr, key) << endl;

  sort(arr.begin(), arr.end());
  cout << "Binary Search: " << binarySearch(arr, key) << endl;
  cout << "Block Search: " << blockSearch(arr, key) << endl;

  return 0;
}