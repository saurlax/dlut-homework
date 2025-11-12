#include <bits/stdc++.h>
using namespace std;

enum TokenType { T_NUM, T_OP, T_LP, T_RP };
struct Token {
  TokenType type;
  double val; // for numbers
  char op;    // for operators + - * /
  Token(TokenType t = T_OP, double v = 0, char o = 0)
      : type(t), val(v), op(o) {}
};

string trim(const string &s) {
  size_t a = s.find_first_not_of(" \t\r\n");
  if (a == string::npos)
    return "";
  size_t b = s.find_last_not_of(" \t\r\n");
  return s.substr(a, b - a + 1);
}

// Parse a number starting at s[i], allowing optional leading sign if provided
// Advances i to first char after the number. Returns {success, value}
pair<bool, double> parseNumber(const string &s, size_t &i) {
  size_t n = s.size();
  size_t j = i;
  bool seenDigit = false;
  bool seenDot = false;
  // optional leading sign handled by caller
  if (j < n && (s[j] == '+' || s[j] == '-')) {
    j++;
  }
  // integer/frac part
  while (j < n && isdigit((unsigned char)s[j])) {
    seenDigit = true;
    j++;
  }
  if (j < n && s[j] == '.') {
    seenDot = true;
    j++;
    while (j < n && isdigit((unsigned char)s[j])) {
      seenDigit = true;
      j++;
    }
  }
  if (!seenDigit)
    return {false, 0.0};
  // exponent
  if (j < n && (s[j] == 'e' || s[j] == 'E')) {
    size_t k = j + 1;
    if (k < n && (s[k] == '+' || s[k] == '-'))
      k++;
    size_t expDigits = 0;
    while (k < n && isdigit((unsigned char)s[k])) {
      expDigits++;
      k++;
    }
    if (expDigits == 0)
      return {false, 0.0};
    j = k;
  }
  string numstr = s.substr(i, j - i);
  // use stod and catch exceptions
  try {
    double v = stod(numstr);
    i = j;
    return {true, v};
  } catch (...) {
    return {false, 0.0};
  }
}

// Tokenize with handling of unary + / - as part of number when appropriate
bool tokenize(const string &s, vector<Token> &out) {
  size_t i = 0, n = s.size();
  TokenType prev = T_OP; // treat as "before any token", so unary allowed
  while (i < n) {
    char c = s[i];
    if (isspace((unsigned char)c)) {
      i++;
      continue;
    }
    if (c == '(') {
      out.emplace_back(T_LP, 0.0, 0);
      prev = T_LP;
      i++;
    } else if (c == ')') {
      out.emplace_back(T_RP, 0.0, 0);
      prev = T_RP;
      i++;
    } else if (c == '+' || c == '-') {
      // decide unary or binary: unary if at start or after '(' or after another
      // operator
      bool unary = (prev == T_OP || prev == T_LP);
      if (unary) {
        // if next is digit or '.', parse signed number
        size_t j = i;
        if (j + 1 < n &&
            (isdigit((unsigned char)s[j + 1]) || s[j + 1] == '.')) {
          auto res = parseNumber(s, j);
          if (!res.first)
            return false;
          out.emplace_back(T_NUM, res.second, 0);
          prev = T_NUM;
          i = j;
        } else {
          // unary + or - not followed by number -> error (we don't support
          // unary before parenthesis like "-(3+4)"? Actually "-(3+4)" is
          // common.) To support "-( ... )", we can interpret unary '-' before
          // '(' as "0 - ( ... )"
          if (i + 1 < n && s[i + 1] == '(') {
            // produce number 0, then binary operator
            out.emplace_back(T_NUM, 0.0, 0);
            out.emplace_back(T_OP, 0.0, c); // + or - as binary op
            prev = T_OP;
            i++;
          } else {
            return false;
          }
        }
      } else {
        out.emplace_back(T_OP, 0.0, c);
        prev = T_OP;
        i++;
      }
    } else if (c == '*' || c == '/') {
      out.emplace_back(T_OP, 0.0, c);
      prev = T_OP;
      i++;
    } else if (isdigit((unsigned char)c) || c == '.') {
      size_t j = i;
      auto res = parseNumber(s, j);
      if (!res.first)
        return false;
      out.emplace_back(T_NUM, res.second, 0);
      prev = T_NUM;
      i = j;
    } else {
      return false;
    }
  }
  return true;
}

int prec(char op) {
  if (op == '+' || op == '-')
    return 1;
  if (op == '*' || op == '/')
    return 2;
  return 0;
}

bool toRPN(const vector<Token> &tokens, vector<Token> &out) {
  vector<Token> ops;
  for (const Token &t : tokens) {
    if (t.type == T_NUM) {
      out.push_back(t);
    } else if (t.type == T_OP) {
      char op = t.op;
      while (!ops.empty() && ops.back().type == T_OP) {
        char top = ops.back().op;
        if ((prec(top) > prec(op)) ||
            (prec(top) == prec(op) /* left-assoc */)) {
          out.push_back(ops.back());
          ops.pop_back();
        } else
          break;
      }
      ops.push_back(t);
    } else if (t.type == T_LP) {
      ops.push_back(t);
    } else if (t.type == T_RP) {
      bool found = false;
      while (!ops.empty()) {
        if (ops.back().type == T_LP) {
          ops.pop_back();
          found = true;
          break;
        } else {
          out.push_back(ops.back());
          ops.pop_back();
        }
      }
      if (!found)
        return false; // mismatched )
    }
  }
  while (!ops.empty()) {
    if (ops.back().type == T_LP || ops.back().type == T_RP)
      return false;
    out.push_back(ops.back());
    ops.pop_back();
  }
  return true;
}

bool evalRPN(const vector<Token> &rpn, double &res) {
  vector<double> st;
  for (const Token &t : rpn) {
    if (t.type == T_NUM)
      st.push_back(t.val);
    else if (t.type == T_OP) {
      if (st.size() < 2)
        return false;
      double b = st.back();
      st.pop_back();
      double a = st.back();
      st.pop_back();
      double v = 0;
      if (t.op == '+')
        v = a + b;
      else if (t.op == '-')
        v = a - b;
      else if (t.op == '*')
        v = a * b;
      else if (t.op == '/') {
        if (b == 0.0)
          return false;
        v = a / b;
      } else
        return false;
      st.push_back(v);
    } else
      return false;
  }
  if (st.size() != 1)
    return false;
  res = st.back();
  return true;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  string line;
  // read entire input as one line (trim trailing newlines)
  if (!getline(cin, line))
    return 0;
  line = trim(line);
  if (line.empty()) {
    cout << "ERROR\n";
    return 1;
  }
  vector<Token> tokens;
  if (!tokenize(line, tokens)) {
    cout << "ERROR\n";
    return 1;
  }
  vector<Token> rpn;
  if (!toRPN(tokens, rpn)) {
    cout << "ERROR\n";
    return 1;
  }
  double result;
  if (!evalRPN(rpn, result)) {
    cout << "ERROR\n";
    return 1;
  }
  // print result with sufficient precision
  cout.setf(std::ios::fmtflags(0), std::ios::floatfield);
  cout << setprecision(15) << result << "\n";
  return 0;
}