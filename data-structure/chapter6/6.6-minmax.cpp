/**
 * 给出一个同时找到 n 个元素中最大元素和最小元素的有效算法，并给出理由
 */
#include <iostream>
#include <vector>
using namespace std;

pair<int, int> minmax(vector<int> &arr) {
  int n = arr.size();
  int min = arr[0];
  int max = arr[0];

  for (int i = 1; i < n; i++) {
    if (arr[i] < min) {
      min = arr[i];
    } else if (arr[i] > max) {
      max = arr[i];
    }
  }

  return make_pair(min, max);
}

int main() {
  vector<int> arr = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
  pair<int, int> result = minmax(arr);

  cout << "Min: " << result.first << endl;
  cout << "Max: " << result.second << endl;

  return 0;
}