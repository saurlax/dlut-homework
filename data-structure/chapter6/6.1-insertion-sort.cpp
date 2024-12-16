#include <iostream>

using namespace std;

void insertionSort(int arr[], int n) {
  for (int i = 1; i < n; i++) {
    int key = arr[i];
    int j = i - 1;
    // 将比 key 大的元素向后移动
    while (j >= 0 && arr[j] > key) {
      arr[j + 1] = arr[j];
      j--;
    }
    arr[j + 1] = key;
  }
}

void binaryInsertionSort(int arr[], int n) {
  for (int i = 1; i < n; i++) {
    int key = arr[i];
    int left = 0;
    int right = i - 1;
    // 在已排序部分中折半查找插入位置
    while (left <= right) {
      int mid = (left + right) / 2;
      if (arr[mid] > key)
        right = mid - 1;
      else
        left = mid + 1;
    }
    // 将大于 key 的元素向后移动
    for (int j = i - 1; j >= left; j--) {
      arr[j + 1] = arr[j];
    }
    arr[left] = key;
  }
}

void shellSort(int arr[], int n) {
  // 选择初始增量 gap
  for (int gap = n / 2; gap > 0; gap /= 2) {
    // 对每个子序列进行直接插入排序
    for (int i = gap; i < n; i++) {
      int key = arr[i];
      int j = i;
      while (j >= gap && arr[j - gap] > key) {
        arr[j] = arr[j - gap];
        j -= gap;
      }
      arr[j] = key;
    }
  }
}

int main() {
  int arr[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
  int n = sizeof(arr) / sizeof(arr[0]);

  insertionSort(arr, n);
  binaryInsertionSort(arr, n);
  shellSort(arr, n);

  for (int i = 0; i < n; i++) {
    cout << arr[i] << " ";
  }
  cout << endl;

  return 0;
}