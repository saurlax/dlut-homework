#include <bits/stdc++.h>
using namespace std;

vector<string> token;
int idx = 0;

string current() { return idx < token.size() ? token[idx] : "$"; }
void advance_token() {
  if (idx < token.size())
    idx++;
}

void match(const string &t) {
  if (current() == t)
    advance_token();
  else {
    cerr << "Error: expected '" << t << "' but found '" << current()
         << "'. Skipping token.\n";
    if (current() != "$")
      advance_token();
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
  if (current() == "+") {
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
  if (current() == "*") {
    cout << "T' -> * F T'\n";
    match("*");
    F();
    T1();
  } else {
    cout << "T' -> e\n";
  }
}

void F() {
  if (current() == "id") {
    cout << "F -> id\n";
    match("id");
  } else if (current() == "(") {
    cout << "F -> ( E )\n";
    match("(");
    E();
    match(")");
  } else {
    cerr << "Error: expected 'id' or '(', but found '" << current()
         << "'. Skipping token.\n";
    if (current() != "$")
      advance_token();
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
  // 读取整个文件到 single string
  ostringstream oss;
  oss << fin.rdbuf();
  string line = oss.str();

  // 把任意标识符视为 id
  for (size_t i = 0; i < line.size();) {
    if (isspace((unsigned char)line[i])) {
      ++i;
      continue;
    }
    if (isalpha((unsigned char)line[i])) {
      size_t j = i + 1;
      while (j < line.size() && isalnum((unsigned char)line[j]))
        ++j;
      token.push_back("id");
      i = j;
    } else {
      char c = line[i++];
      if (c == '+' || c == '*' || c == '(' || c == ')') {
        token.push_back(string(1, c));
      } else {
        // 忽略其他字符
      }
    }
  }
  token.push_back("$"); // 结束标记

  E();
  if (current() == "$") {
    cout << "Parsing finished: input accepted (or no unrecoverable errors "
            "encountered).\n";
  } else {
    cerr << "Parsing finished: leftover tokens starting at '" << current()
         << "'.\n";
  }
  return 0;
}