#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

struct Term {
  int coefficient;
  map<string, int> variables;
};

Term parseTerm(const string &str) {
  Term term;
  term.coefficient = 1;
  stringstream ss(str);
  char c;
  while (ss >> c) {
    if (isdigit(c)) {
      ss.unget();
      ss >> term.coefficient;
    } else if (isalpha(c)) {
      string var(1, c);
      if (ss.peek() == '^') {
        ss.get();
        int exp;
        ss >> exp;
        term.variables[var] = exp;
      } else {
        term.variables[var] = 1;
      }
    }
  }
  return term;
}

bool compareVariables(const Term &t1, const Term &t2) {
  return t1.variables == t2.variables;
}

void printTerm(const Term &term) {
  cout << term.coefficient;
  for (const auto &var : term.variables) {
    cout << var.first;
    if (var.second != 1) {
      cout << "^" << var.second;
    }
  }
}

vector<Term> parsePolynomial(const string &poly) {
  vector<Term> terms;
  string s = poly;
  if (s[0] != '+' && s[0] != '-') {
    s = '+' + s; // 保证字符串以符号开头
  }
  size_t pos = 0;
  while (pos < s.length()) {
    char sign = s[pos];
    pos++;
    size_t nextPos = s.find_first_of("+-", pos);
    string termStr = s.substr(pos, nextPos - pos);
    Term term = parseTerm(termStr);
    if (sign == '-') {
      term.coefficient = -term.coefficient;
    }
    terms.push_back(term);
    pos = nextPos;
  }
  return terms;
}

void sumPolynomial(vector<Term> &terms) {
  for (size_t i = 0; i < terms.size(); i++) {
    for (size_t j = i + 1; j < terms.size(); j++) {
      if (compareVariables(terms[i], terms[j])) {
        terms[i].coefficient += terms[j].coefficient;
        terms.erase(terms.begin() + j);
        j--;
      }
    }
  }
}

void printPolynomial(const vector<Term> &terms) {
  for (size_t i = 0; i < terms.size(); i++) {
    const Term &term = terms[i];
    if (term.coefficient < 0) {
      cout << "-";
    } else if (i > 0) {
      cout << "+";
    }
    if (abs(term.coefficient) != 1 || term.variables.empty()) {
      cout << abs(term.coefficient);
    } else if (term.coefficient == -1) {
      cout << "-";
    }
    for (const auto &var : term.variables) {
      cout << var.first;
      if (var.second != 1) {
        cout << "^" << var.second;
      }
    }
  }
  cout << endl;
}

// 主函数
int main() {
  string input;
  cout << "请输入多项式（例如：2x^3y+4x^3y）：";
  cin >> input;

  vector<Term> terms = parsePolynomial(input);
  sumPolynomial(terms);

  cout << "求和结果：";
  printPolynomial(terms);

  return 0;
}