/**
 * 擦，我们的密码设置才奇葩呢。
 * 要求：
 * （1）必须要有字母（大小写都有）、数字、符号，八个以上字符
 * （2）密码每三个月重新设置一次，新设置的密码跟以前所有的密码重复的字符不超过三个
 * 
 * 没几次搞下来就无法设自己熟悉的字符串了，只好随机创建一个，写到纸上每次抄进去。
 * 假设旧密码为：by99YL17!
 * 利用正规式检测输入的新密码是否符合规定。
 */

#include <iostream>
#include <string>
#include <regex>
#include <unordered_set>

int main()
{
  using namespace std;
  const string oldPwd = "by99YL17!";
  unordered_set<char> oldSet(oldPwd.begin(), oldPwd.end());

  const regex hasLower("[a-z]");
  const regex hasUpper("[A-Z]");
  const regex hasDigit("[0-9]");
  const regex hasSymbol("[^A-Za-z0-9]");

  string s;
  while (getline(cin, s))
  {
    bool ok = true;

    if (s.size() < 8)
      ok = false;
    if (ok && !regex_search(s, hasLower))
      ok = false;
    if (ok && !regex_search(s, hasUpper))
      ok = false;
    if (ok && !regex_search(s, hasDigit))
      ok = false;
    if (ok && !regex_search(s, hasSymbol))
      ok = false;

    if (ok)
    {
      unordered_set<char> newSet;
      for (char c : s)
        newSet.insert(c);
      int common = 0;
      for (char c : newSet)
        if (oldSet.count(c))
          ++common;
      if (common > 3)
        ok = false;
    }

    cout << (ok ? "Legal password" : "Illegal password") << "\n";
  }
  return 0;
}
