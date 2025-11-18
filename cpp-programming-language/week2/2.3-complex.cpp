/**
 * 设计并测试Complex类，进行复数的四则运算。编写一个主程序，测试这个类。
 */

#include <iostream>
using namespace std;

class Complex {
  double real = 0, imag;

 public:
  Complex(double real = 0, double imag = 0) : real(real), imag(imag) {}
  void set(double real = 0, double imag = 0) {
    this->real = real;
    this->imag = imag;
  }
  void show() { printf("%g%+gi\n", real, imag); }
  Complex operator+(Complex c) { return Complex(real + c.real, imag + c.imag); }
  Complex operator-(Complex c) { return Complex(real - c.real, imag - c.imag); }
  Complex operator*(Complex c) {
    return Complex(real * c.real - imag * c.imag,
                   real * c.imag + imag * c.real);
  }
  Complex operator/(Complex c) {
    return Complex(
        (real * c.real + imag * c.imag) / (c.real * c.real + c.imag * c.imag),
        (imag * c.real - real * c.imag) / (c.real * c.real + c.imag * c.imag));
  }
};

int main() {
  Complex c1, c2(0, 1);
  c1.set(1, 2);
  c1.show();
  c2.show();
  (c1 + c2).show();
  (c1 - c2).show();
  (c1 * c2).show();
  (c1 / c2).show();
  return 0;
}