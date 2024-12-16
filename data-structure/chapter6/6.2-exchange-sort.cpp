#include <iostream>

using namespace std;

void bubbleSort(int arr[], int n) {
  for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - i - 1; j++) {
      if (arr[j] > arr[j + 1]) {
        swap(arr[j], arr[j + 1]);
      }
    }
  }
}

int partition(int arr[], int low, int high) {
  int pivot = arr[low];
  int left = low + 1;
  int right = high;

  while (true) {
    while (left <= right && arr[left] <= pivot) {
      left++;
    }
    while (left <= right && arr[right] >= pivot) {
      right--;
    }
    if (left > right) {
      break;
    }
    swap(arr[left], arr[right]);
  }
  swap(arr[low], arr[right]);
  return right;
}

void quickSort(int arr[], int low, int high) {
  if (low < high) {
    int pi = partition(arr, low, high); // 分区索引
    quickSort(arr, low, pi - 1);
    quickSort(arr, pi + 1, high);
  }
}

int main() {
  int arr[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
  int n = sizeof(arr) / sizeof(arr[0]);

  bubbleSort(arr, n);
  quickSort(arr, 0, n - 1);

  for (int i = 0; i < n; i++) {
    cout << arr[i] << " ";
  }
  cout << endl;

  return 0;
}