/**
 * 定义一个集合类Set，最多存放10个不重复的整数，实现集合的如下操作：
 * 1）向集合增加某个整型元素时，保证集合中没有重复元素
 * 2) 删除指定的元素，查找该元素在集合中则从集合中删除该元素；
 * 3）重载运算符“+”，实现两个集合对象的合并操作
 * 4）重载运算符“*”，求两个集合对象的交集操作
 */

#include <iostream>
using namespace std;

class Set {
  int data[10];
  int size;

 public:
  Set() : size(0) {}
  bool add(int n) {
    if (size == 10) return false;
    for (int i = 0; i < size; i++) {
      if (data[i] == n) return false;
    }
    data[size++] = n;
    return true;
  }
  bool remove(int n) {
    for (int i = 0; i < size; i++) {
      if (data[i] == n) {
        for (int j = i; j < size - 1; j++) {
          data[j] = data[j + 1];
        }
        size--;
        return true;
      }
    }
    return false;
  }
  Set operator+(const Set& other) {
    Set s;
    for (int i = 0; i < size; i++) {
      s.add(data[i]);
    }
    for (int i = 0; i < other.size; i++) {
      s.add(other.data[i]);
    }
    return s;
  }
  Set operator*(const Set& other) {
    Set s;
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < other.size; j++) {
        if (data[i] == other.data[j]) {
          s.add(data[i]);
          break;
        }
      }
    }
    return s;
  }
  friend ostream& operator<<(ostream& os, const Set& s) {
    os << "{";
    for (int i = 0; i < s.size; i++) {
      os << s.data[i];
      if (i < s.size - 1) os << ", ";
    }
    os << "}";
    return os;
  }
};

int main() {
  Set s1, s2, s3;
  s1.add(1);
  s1.add(2);
  s1.add(3);
  cout << "s1: " << s1 << endl;
  s2.add(3);
  s2.add(4);
  cout << "s2: " << s2 << endl;
  s3 = s1 + s2;
  cout << "s1+s2: " << s3 << endl;
  s3 = s1 * s2;
  cout << "s1*s2: " << s3 << endl;
  return 0;
}