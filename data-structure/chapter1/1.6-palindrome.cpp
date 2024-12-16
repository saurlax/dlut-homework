/**
 * 应用栈结构，判断是否回文
 */

#include <iostream>
#include <stack>
#include <string>
using namespace std;

bool is_palindrome(const string &s) {
  stack<char> st;
  for (int i = 0; i < s.length() / 2; i++)
    st.push(s[i]);
  for (int i = (s.length() + 1) / 2; i < s.length(); i++) {
    if (s[i] != st.top())
      return false;
    st.pop();
  }
  return true;
}

int main() {
  string s;
  cin >> s;
  cout << is_palindrome(s);
  return 0;
}