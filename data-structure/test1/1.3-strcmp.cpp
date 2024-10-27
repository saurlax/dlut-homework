/**
 * 设计 strcmp(s, t) 函数，比较字符串 s 和 t 的大小
 */

#include <iostream>
#include <string>
using namespace std;

int strcmp(const string &s, const string &t) {
  int i = 0;
  while (i < s.length() && i < t.length()) {
    if (s[i] < t[i])
      return -1;
    else if (s[i] > t[i])
      return 1;
    i++;
  }
  if (s.length() < t.length())
    return -1;
  else if (s.length() > t.length())
    return 1;
  return 0;
}

int main() {
  string s, t;
  cin >> s >> t;
  cout << strcmp(s, t);
  return 0;
}