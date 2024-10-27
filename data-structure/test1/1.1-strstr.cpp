/**
 * 实现简单(朴素)的字符串匹配算法
 */

#include <iostream>
using namespace std;
int strstr(const string &s, const string &t) {
  if (t.length() > s.length())
    return -1;
  for (int i = 0; i <= s.length() - t.length(); i++) {
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
  cout << strstr(s, t);
  return 0;
}