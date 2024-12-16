/**
 * 改进快速排序的另一种方法，就是使用待排序列的一小部分元素的中位数来分割子序列，例如三取样切分就是将取样大小设为
 * 3 并取大小居中的元素进行分割。请实现三取样切分快速排序算法，并运行双倍测试。
 */

#include <chrono>
#include <iostream>
#include <vector>
#define N 1000

using namespace std;

int partition(vector<int> &arr, int low, int high) {
  int mid = low + (high - low) / 2;
  int firstMid = mid - 1;
  int secondMid = mid;
  int thirdMid = mid + 1;

  // 确保索引不越界
  if (firstMid < low)
    firstMid = low;
  if (thirdMid > high)
    thirdMid = high;

  // 选择中间三个元素的值
  int a = arr[firstMid];
  int b = arr[secondMid];
  int c = arr[thirdMid];

  // 求三个元素的中位数作为枢轴
  int pivot;
  if ((a <= b && b <= c) || (c <= b && b <= a))
    pivot = b;
  else if ((b <= a && a <= c) || (c <= a && a <= b))
    pivot = a;
  else
    pivot = c;

  // 开始 partition
  while (low <= high) {
    while (arr[low] < pivot)
      low++;
    while (arr[high] > pivot)
      high--;
    if (low <= high) {
      swap(arr[low], arr[high]);
      low++;
      high--;
    }
  }
  return low;
}

void quickSort(vector<int> &arr, int low, int high) {
  if (low < high) {
    int pi = partition(arr, low, high);
    quickSort(arr, low, pi - 1);
    quickSort(arr, pi, high);
  }
}

void benchmark(int n) {
  vector<int> arr(n);
  for (int i = 0; i < n; i++) {
    arr[i] = rand();
  }

  auto start = chrono::steady_clock::now();
  quickSort(arr, 0, n - 1);
  auto end = chrono::steady_clock::now();

  cout << "排序 " << n << " 个元素所用时间："
       << chrono::duration_cast<chrono::microseconds>(end - start).count()
       << " ms" << endl;
}

int main() {
  benchmark(N);
  benchmark(2 * N);
  return 0;
}