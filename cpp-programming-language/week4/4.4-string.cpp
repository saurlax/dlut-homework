/**
 * 完善“类与对象作业”中的String类，使其能够执行以下运算。
 * s1, s2, s3是String类的三个对象。
 * 表达式s1=s2+s3表示将s2与s3连接成一个新的字符串并赋给s1。
 * 请重载必要的运算符。
 */

#include <cstring>
#include <iostream>
using namespace std;

class String {
  char* m_data;

 public:
  String(const char* s = "") {
    m_data = new char[strlen(s) + 1];
    strcpy(m_data, s);
  }
  String(const String& other) {
    m_data = new char[strlen(other.m_data) + 1];
    strcpy(m_data, other.m_data);
  }
  ~String() { delete[] m_data; }
  void set(const char* s) {
    delete[] m_data;
    m_data = new char[strlen(s) + 1];
    strcpy(m_data, s);
  }
  String operator+(const String& other) {
    String s;
    s.m_data = new char[strlen(m_data) + strlen(other.m_data) + 1];
    strcpy(s.m_data, m_data);
    strcat(s.m_data, other.m_data);
    return s;
  }
  String& operator=(const String& other) {
    if (this == &other) return *this;
    delete[] m_data;
    m_data = new char[strlen(other.m_data) + 1];
    strcpy(m_data, other.m_data);
    return *this;
  }
  void print() { cout << m_data << endl; }
};

int main() {
  String s1("hello "), s2("world!"), s3;
  s3 = s1 + s2;
  s3.print();
  return 0;
}