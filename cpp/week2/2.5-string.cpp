/**
 * 编写并测试字符串类String
 */

#include <cstring>
#include <iostream>
using namespace std;

class String {
  char* m_data;

 public:
  String(const char* s = NULL) {
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
  void print() { cout << m_data << endl; }
};

int main() {
  String s1("Hello"), s2(s1);
  s2.print();
  s1.set("World");
  s1.print();
  return 0;
}