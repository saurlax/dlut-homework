/**
 * 任意输入一个析取范式，计算并输出其主析取范式
 */

#include <bits/stdc++.h>
using namespace std;

set<char> vars;
vector<bool> table;

char precedence(char op) {
  switch (op) {
    case '!':
      return 3;
    case '&':
      return 2;
    case '|':
      return 1;
    default:
      return 0;
  }
}

bool applyOp(bool a, bool b, char op) {
  switch (op) {
    case '&':
      return a & b;
    case '|':
      return a | b;
  }
  return 0;
}

bool applyNot(bool a) { return !a; }

bool eval(const string &tokens) {
  std::stack<bool> values;
  std::stack<char> ops;

  for (char token : tokens) {
    switch (token) {
      case ' ':
        continue;
      case '0':
      case '1':
        values.push(token == '1');
        break;
      case '(':
        ops.push(token);
        break;
      case ')':
        while (!ops.empty() && ops.top() != '(') {
          if (ops.top() == '!') {
            bool value = values.top();
            values.pop();
            values.push(applyNot(value));
            ops.pop();
          } else {
            bool val2 = values.top();
            values.pop();
            bool val1 = values.top();
            values.pop();
            char op = ops.top();
            ops.pop();
            values.push(applyOp(val1, val2, op));
          }
        }
        if (!ops.empty()) ops.pop();
        break;
      case '!':
      case '&':
      case '|':
        while (!ops.empty() && precedence(ops.top()) >= precedence(token)) {
          if (ops.top() == '!') {
            bool value = values.top();
            values.pop();
            values.push(applyNot(value));
            ops.pop();
          } else {
            bool val2 = values.top();
            values.pop();
            bool val1 = values.top();
            values.pop();
            char op = ops.top();
            ops.pop();
            values.push(applyOp(val1, val2, op));
          }
        }
        ops.push(token);
        break;
    }
  }

  while (!ops.empty()) {
    if (ops.top() == '!') {
      bool value = values.top();
      values.pop();
      values.push(applyNot(value));
      ops.pop();
    } else {
      bool val2 = values.top();
      values.pop();
      bool val1 = values.top();
      values.pop();
      char op = ops.top();
      ops.pop();
      values.push(applyOp(val1, val2, op));
    }
  }
  return values.top();
}

int main() {
  string prop;
  while (true) {
    vars.clear();
    table.clear();
    cin >> prop;

    for (size_t i = 0; i < prop.size(); i++) {
      if (prop[i] == '~') {
        prop.replace(i, 1, 1, '!');
      } else if (prop.substr(i, 2) == "/\\") {
        prop.replace(i, 2, 1, '&');
      } else if (prop.substr(i, 2) == "\\/") {
        prop.replace(i, 2, 1, '|');
      }
    }

    for (char c : prop) {
      if (isalpha(c)) vars.insert(c);
    }

    int max = 1 << vars.size();
    for (int state = 0; state < max; state++) {
      string token = prop;
      int v = 0;
      for (char var : vars) {
        char value = ((state >> v) & 1) ? '1' : '0';
        for (int i = 0; i < token.size(); i++) {
          if (token[i] == var) token.replace(i, 1, 1, value);
        }
        v++;
      }
      table.push_back(eval(token));
    }

    string pdnf;
    bool first = true;
    for (int state = 0; state < max; state++) {
      if (table[state]) {
        if (first) {
          first = false;
        } else {
          pdnf += ", ";
        }
        pdnf += to_string(state);
      }
    }
    printf("主析取范式：∑(%s)\n", pdnf.c_str());
  }
  return 0;
}