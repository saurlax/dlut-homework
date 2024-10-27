/**
 * 将 Array 类的成员 m_data 的访问权限改为 protected，
 * 之后基于 Array 类完成该类的两个派生类。在主函数中对编写的派生类进行测试。
 * （1）派生类 ave_array 提供 average() 函数，用于计算数组元素的平均值。
 * （2）派生类 rev_array 提供 reverse() 函数, 使数组元素逆序。
 */

#include <iostream>
using namespace std;

class Array {
 protected:
  int n;
  int* m_data;

 public:
  Array(int n = 0) : n(n) {
    m_data = new int[n];
    for (int i = 0; i < n; i++) m_data[i] = 0;
  }

  Array(const Array& other) : n(other.n) {
    m_data = new int[n];
    for (int i = 0; i < n; i++) m_data[i] = other.m_data[i];
  }

  ~Array() { delete[] m_data; }

  int& operator[](int i) { return m_data[i]; }

  Array& operator=(const Array& other) {
    if (this == &other) return *this;
    delete[] m_data;
    n = other.n;
    m_data = new int[n];
    for (int i = 0; i < n; i++) m_data[i] = other.m_data[i];
    return *this;
  }

  friend ostream& operator<<(ostream& os, const Array& arr) {
    for (int i = 0; i < arr.n; i++) os << arr.m_data[i] << ' ';
    return os;
  }

  friend istream& operator>>(istream& is, Array& arr) {
    for (int i = 0; i < arr.n; i++) is >> arr.m_data[i];
    return is;
  }
};

class AveArray : public Array {
 public:
  AveArray(int n = 0) : Array(n) {}
  double average() {
    double sum = 0;
    for (int i = 0; i < n; i++) sum += m_data[i];
    return sum / n;
  }
};

class RevArray : public Array {
 public:
  RevArray(int n = 0) : Array(n) {}
  void reverse() {
    for (int i = 0; i < n / 2; i++) {
      int temp = m_data[i];
      m_data[i] = m_data[n - i - 1];
      m_data[n - i - 1] = temp;
    }
  }
};

int main() {
  AveArray arr1(5);
  cin >> arr1;
  cout << "Average: " << arr1.average() << endl;
  RevArray arr2(5);

  cin >> arr2;
  arr2.reverse();
  cout << arr2 << endl;
  return 0;
}