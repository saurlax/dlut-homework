/**
 * 应用栈结构，判断分隔符是否匹配
 */

#include <iostream>
#include <stack>
#include <string>
using namespace std;

bool is_bracket_closed(const string &s) {
  stack<char> st;
  for (int i = 0; i < s.length(); i++) {
    if (s[i] == '(' || s[i] == '[' || s[i] == '{')
      st.push(s[i]);
    else if (s[i] == ')') {
      if (st.empty() || st.top() != '(')
        return false;
      st.pop();
    } else if (s[i] == ']') {
      if (st.empty() || st.top() != '[')
        return false;
      st.pop();
    } else if (s[i] == '}') {
      if (st.empty() || st.top() != '{')
        return false;
      st.pop();
    }
  }
  return st.empty();
}

int main() {
  string s;
  cin >> s;
  cout << is_bracket_closed(s);
  return 0;
}