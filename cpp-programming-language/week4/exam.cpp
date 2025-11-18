/**
 * 使用C++的数组实现“长度可变数组类”MyArray，该类结构如下。
 * 为该类添加必要的函数并重载所需的运算符，使主函数能够运行并得到目标输出。
 * 不可以修改MyArray结构，不可以修改main函数。
 * 提示：可以分段完成，先完成arr1相关，再完成arr2相关，最后完成arr3。
 */

#include <iostream>
using namespace std;

class MyArray {
 public:
  MyArray(int s = 0) : size(s) { arr = new int[size]; }
  MyArray(const MyArray& a) : size(a.size) {
    arr = new int[size];
    for (int i = 0; i < size; i++) {
      arr[i] = a.arr[i];
    }
  }
  MyArray operator=(const MyArray& a) {
    if (this == &a) return *this;
    delete[] arr;
    size = a.size;
    arr = new int[size];
    for (int i = 0; i < size; i++) {
      arr[i] = a.arr[i];
    }
    return *this;
  }
  int& operator[](int i) { return arr[i]; }
  int operator()() { return size; }
  MyArray operator--(int) {
    MyArray temp(*this);
    if (size > 0) {
      size--;
      int* _arr = new int[size];
      for (int i = 0; i < size; i++) {
        _arr[i] = arr[i];
      }
      delete[] arr;
      arr = _arr;
    }
    return temp;
  }
  MyArray& operator++() {
    size++;
    int* _arr = new int[size];
    for (int i = 0; i < size - 1; i++) {
      _arr[i] = arr[i];
    }
    delete[] arr;
    arr = _arr;
    return *this;
  }
  MyArray operator+(const MyArray& a) {
    MyArray temp(size + a.size);
    for (int i = 0; i < size; i++) {
      temp[i] = arr[i];
    }
    for (int i = 0; i < a.size; i++) {
      temp[size + i] = a.arr[i];
    }
    return temp;
  }
  friend ostream& operator<<(ostream& os, const MyArray& a) {
    for (int i = 0; i < a.size; i++) {
      os << a.arr[i] << " ";
    }
    return os;
  }
  ~MyArray() { delete[] arr; }

 private:
  int* arr;  // 指向动态分配的数组的指针
  int size;  // 数组的大小
};

// 不要修改main函数
int main() {
  // 创建一个size是3的整数数组
  MyArray arr1(3);
  arr1[0] = 0;
  arr1[1] = 1;
  arr1[2] = 2;

  // 输出数组1的大小和所有元素
  cout << "Array 1 size: " << arr1() << endl;
  cout << "Array 1 contents: " << arr1 << endl;

  // 数组大小-1,并丢掉最后一个元素
  arr1--;

  MyArray arr2(2);
  arr2[0] = 10;
  arr2[1] = 11;

  // 数组大小+2
  ++ ++arr2;
  // 数组最后两个元素赋值
  arr2[arr2() - 2] = 12;
  arr2[arr2() - 1] = 13;

  // 输出数组2的大小和所有元素
  cout << "Array 2 size: " << arr2() << endl;
  cout << "Array 2 contents: " << arr2 << endl;

  // 连接两个数组成一个新的数组
  MyArray arr3;
  arr3 = arr1 + arr2;
  cout << "Array 3 contents: " << arr3 << endl;
  cout << "Array 3 size: " << arr3() << endl;

  return 0;
}
