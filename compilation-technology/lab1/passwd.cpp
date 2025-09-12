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
