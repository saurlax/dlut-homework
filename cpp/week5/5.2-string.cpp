/**
 * 将 String 类的成员 m_data 的访问权限改为 protected，
 * 之后基于 String 类完成该类的两个派生类。在主函数中对编写的派生类进行测试。
 * （1）派生类 String1 提供以下函数：
 * int getLength(); // 获得字符串长度，不含\0
 * bool find(const char *c); // 检查是否包含参数字符串形式的子串
 * （2）派生类 String2 提供以下函数：
 * bool replace(char old, char new); // 用 new 替换所有 old，返回是否替换成功
 * bool delete(char c); // 删掉所有 c 字符，返回是否删除成功
 */

#include <cstring>
#include <iostream>
using namespace std;

class String {
 protected:
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
  String& operator=(const String& other) {
    if (this == &other) return *this;
    delete[] m_data;
    m_data = new char[strlen(other.m_data) + 1];
    strcpy(m_data, other.m_data);
    return *this;
  }
  friend ostream& operator<<(ostream& os, const String& s) {
    os << s.m_data;
    return os;
  }
};

class String1 : public String {
 public:
  String1(const char* s = NULL) : String(s) {}
  int getLength() { return strlen(m_data); }
  bool find(const char* c) { return strstr(m_data, c) != NULL; }
};

class String2 : public String {
 public:
  String2(const char* s = NULL) : String(s) {}
  bool replace(char oldstr, char newstr) {
    bool found = false;
    for (int i = 0; m_data[i]; i++)
      if (m_data[i] == oldstr) {
        m_data[i] = newstr;
        found = true;
      }
    return found;
  }
  bool deleteChar(char c) {
    bool found = false;
    int j = 0;
    for (int i = 0; m_data[i]; i++)
      if (m_data[i] != c) m_data[j++] = m_data[i];
    m_data[j] = '\0';
    return found;
  }
};

int main() {
  String1 s1("hello");
  cout << s1.getLength() << endl;
  cout << s1.find("ell") << endl;
  String2 s2("hello");
  cout << s2.replace('l', 'L') << endl;
  cout << s2.deleteChar('o') << endl;
  cout << s2 << endl;
  return 0;
}