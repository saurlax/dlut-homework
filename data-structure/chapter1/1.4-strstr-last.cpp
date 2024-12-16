/**
 * 设计一个算法，求串 str 中 substr 最后一次出现的位置
 */

#include <iostream>
#include <string>
using namespace std;

int strstr_last(const string &s, const string &t) {
  if (t.length() > s.length())
    return -1;
  for (int i = s.length() - t.length(); i >= 0; i--) {
    if (s[i] == t[0]) {
      int j = i, k = 0;
      while (s[j] == t[k] && k < t.length()) {
        j++;
        k++;
      }
      if (k == t.length())
        return i;
    }
  }
  return -1;
}

int main() {
  string s, t;
  cin >> s >> t;
  cout << strstr_last(s, t);
  return 0;
}