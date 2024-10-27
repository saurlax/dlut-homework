/**
 * （1）定义函数模板min，该模板可以返回数组中的最小元素。
 * 定义该模板，然后使用int数组、Box数组对该模板进行测试。
 * 注：Box具有长宽高，需要完成必要的运算符重载。
 * （2）根据以下类模板SafeArray的设计，完成该类模板的完整定义。
 * 注意：所有的函数都要求在类外定义。
 * 在主函数中用int序列和Box序列对SafeArray模板进行测试。
 */

#include <iostream>
using namespace std;

template <typename T>
T min(T arr[], int n) {
  T min = arr[0];
  for (int i = 1; i < n; i++) {
    if (arr[i] < min) {
      min = arr[i];
    }
  }
  return min;
}

template <typename T>
class SafeArray {
 public:
  SafeArray(int s);
  // 构造函数
  T& operator[](int index);
  // 根据索引访问某一元素，注意：该下标访问越界应有提示
  void sort();
  // 升序排序（使用插入排序）
  T sum();
  // 数组元素求和
  void print();
  // 输出数组内容

 protected:
  int size;
  T* element;
};

template <typename T>
SafeArray<T>::SafeArray(int s) : size(s) {
  element = new T[size];
}

template <typename T>
T& SafeArray<T>::operator[](int index) {
  if (index < 0 || index >= size) {
    cout << "Index out of range!" << endl;
    return element[0];
  }
  return element[index];
}

template <typename T>
void SafeArray<T>::sort() {
  for (int i = 0; i < size - 1; i++) {
    for (int j = 0; j < size - i - 1; j++) {
      if (element[j] > element[j + 1]) {
        T temp = element[j];
        element[j] = element[j + 1];
        element[j + 1] = temp;
      }
    }
  }
}

template <typename T>
T SafeArray<T>::sum() {
  T sum = 0;
  for (int i = 0; i < size; i++) sum += element[i];
  return sum;
}

template <typename T>
void SafeArray<T>::print() {
  for (int i = 0; i < size; i++) cout << element[i] << " ";
  cout << endl;
}

class Box {
 public:
  Box(int l = 0, int w = 0, int h = 0) : l(l), w(w), h(h) {}
  int getVolume() const { return l * w * h; }
  bool operator<(const Box& b) { return getVolume() < b.getVolume(); }
  Box& operator+=(const Box& b) {
    l += b.l;
    w += b.w;
    h += b.h;
    return *this;
  }
  friend ostream& operator<<(ostream& os, const Box& b) {
    printf("Box(%d, %d, %d)", b.l, b.w, b.h);
    return os;
  }

 private:
  int l, w, h;
};

int main() {
  int arr1[] = {1, 2, 3, 4, 5};
  cout << min(arr1, 5) << endl;
  Box arr2[] = {Box(1, 2, 3), Box(2, 3, 4), Box(3, 4, 5)};
  cout << min(arr2, 3) << endl;

  SafeArray<int> sa1(5);
  for (int i = 0; i < 5; i++) sa1[i] = 5 - i;
  sa1.print();
  sa1.sort();
  sa1.print();
  SafeArray<Box> sa2(3);
  for (int i = 0; i < 3; i++) sa2[i] = Box(i, i + 1, i + 2);
  sa2.print();
  cout << sa2.sum() << endl;
  return 0;
}