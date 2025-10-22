#include <bits/stdc++.h>
using namespace std;

vector<string> tok;
int idx = 0;

string cur() { return idx < (int)tok.size() ? tok[idx] : "$"; }
void advanceTok() {
  if (idx < (int)tok.size())
    ++idx;
}

void match(const string &t) {
  if (cur() == t)
    advanceTok();
  else {
    cerr << "Error: expected '" << t << "' but found '" << cur()
         << "'. Skipping token.\n";
    if (cur() != "$")
      advanceTok();
  }
}

void E();
void E1();
void T();
void T1();
void F();

void E() {
  cout << "E -> T E'\n";
  T();
  E1();
}

void E1() {
  if (cur() == "+") {
    cout << "E' -> + T E'\n";
    match("+");
    T();
    E1();
  } else {
    cout << "E' -> e\n";
  }
}

void T() {
  cout << "T -> F T'\n";
  F();
  T1();
}

void T1() {
  if (cur() == "*") {
    cout << "T' -> * F T'\n";
    match("*");
    F();
    T1();
  } else {
    cout << "T' -> e\n";
  }
}

void F() {
  if (cur() == "id") {
    cout << "F -> id\n";
    match("id");
  } else if (cur() == "(") {
    cout << "F -> ( E )\n";
    match("(");
    E();
    match(")");
  } else {
    cerr << "Error: expected 'id' or '(', but found '" << cur()
         << "'. Skipping token.\n";
    if (cur() != "$")
      advanceTok();
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ifstream fin("input.txt");
  if (!fin) {
    cerr << "Cannot open input.txt\n";
    return 1;
  }
  // 读取整个文件到 single string（保留空格换行作为分隔）
  ostringstream oss;
  oss << fin.rdbuf();
  string line = oss.str();

  // 词法简单化：把任意标识符视为 id
  for (size_t i = 0; i < line.size();) {
    if (isspace((unsigned char)line[i])) {
      ++i;
      continue;
    }
    if (isalpha((unsigned char)line[i])) {
      size_t j = i + 1;
      while (j < line.size() && isalnum((unsigned char)line[j]))
        ++j;
      tok.push_back("id");
      i = j;
    } else {
      char c = line[i++];
      if (c == '+' || c == '*' || c == '(' || c == ')') {
        tok.push_back(string(1, c));
      } else {
        // 忽略其他字符
      }
    }
  }
  tok.push_back("$"); // 结束标记

  E();
  if (cur() == "$") {
    cout << "Parsing finished: input accepted (or no unrecoverable errors "
            "encountered).\n";
  } else {
    cerr << "Parsing finished: leftover tokens starting at '" << cur()
         << "'.\n";
  }
  return 0;
}