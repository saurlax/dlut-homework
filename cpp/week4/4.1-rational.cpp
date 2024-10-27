/**
 * 完善“类与对象作业”的Rational类，使其能够执行以下运算。
 * （1）重载比较运算符>、 >=、< 、<=、== 和 !=，实现两个有理数比较。
 * （2）重载运算符“>>”，使其能以分别输入分子和分母的方式输入分数。注意分母不能为0。
 * （3）重载运算符“<<”，使其能以规范的方式输出分数，如1/2，-1/3，注意分母不能为0。
 */

#include <iostream>
using namespace std;

class Rational {
  int n, m;
  void simple() {
    int a = m, b = n, t;
    while (b != 0) t = a % b, a = b, b = t;
    m /= a, n /= a;
    if (n < 0) m = -m, n = -n;
  }

 public:
  Rational(int n = 0, int m = 1) : n(n) {
    if (m == 0) m = 1;
    this->m = m;
  }
  Rational operator+(Rational &r) {
    return Rational(n * r.m + m * r.n, m * r.m);
  }
  Rational operator-(Rational &r) {
    return Rational(n * r.m - m * r.n, m * r.m);
  }
  Rational operator*(Rational &r) { return Rational(n * r.n, m * r.m); }
  Rational operator/(Rational &r) { return Rational(n * r.m, m * r.n); }
  bool operator>(Rational &r) { return n * r.m > m * r.n; }
  bool operator>=(Rational &r) { return n * r.m >= m * r.n; }
  bool operator<(Rational &r) { return n * r.m < m * r.n; }
  bool operator<=(Rational &r) { return n * r.m <= m * r.n; }
  bool operator==(Rational &r) { return n * r.m == m * r.n; }
  bool operator!=(Rational &r) { return n * r.m != m * r.n; }
  friend istream &operator>>(istream &in, Rational &r) {
    in >> r.n >> r.m;
    if (r.m == 0) r.m = 1;
    return in;
  }
  friend ostream &operator<<(ostream &out, Rational &r) {
    r.simple();
    out << r.n << '/' << r.m;
    return out;
  }
};

int main() {
  Rational r1(1, 2), r2(3, 4);
  cout << (r1 > r2) << endl;
  cout << (r1 >= r2) << endl;
  cout << (r1 < r2) << endl;
  cout << (r1 <= r2) << endl;
  cout << (r1 == r2) << endl;
  cout << (r1 != r2) << endl;
  cin >> r1;
  cout << r1 << endl;
  return 0;
}