/**
 * 定义一个分数类如下，要求实现各个成员函数，并在主函数中测试两个分数的加减乘除等运算。
 */

#include <iostream>
using namespace std;

class Rational {
  int m, n;
  void simple() {
    int a = m, b = n, t;
    while (b != 0) {
      t = a % b;
      a = b;
      b = t;
    }
    m /= a;
    n /= a;
    if (n < 0) {
      m = -m;
      n = -n;
    }
  }

 public:
  Rational(int = 0, int n = 1) : m(m), n(n) {}
  Rational R_add(Rational r) { return Rational(m * r.n + n * r.m, n * r.n); }
  Rational R_sub(Rational r) { return Rational(m * r.n - n * r.m, n * r.n); }
  Rational R_mul(Rational r) { return Rational(m * r.m, n * r.n); }
  Rational R_div(Rational r) { return Rational(m * r.n, n * r.m); }
  void print() {
    this->simple();
    cout << m << "/" << n << endl;
  }
};

int main() {
  Rational r1(1, 3), r2(1, 2);
  r1.print();
  r2.print();
  r1.R_add(r2).print();
  r1.R_sub(r2).print();
  r1.R_mul(r2).print();
  r1.R_div(r2).print();
  return 0;
}