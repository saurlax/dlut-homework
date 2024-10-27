/**
 * 完善“类与对象作业”中的Time类，使其能够执行以下运算。
 * （1）重载运算符“+”，能够实现时间对象与整数秒的的加操作。注意应符合交换律。
 * （2）重载运算符“<<”输出时间对象，能够按照某方式显示时间。
 * （3）重载运算符“++”，要求能够实现时间的合理自增功能(秒数的增减)。注意前置与后置。
 */

#include <iostream>

using namespace std;

class Time {
  int hour, minute, second;

 public:
  void set(int h, int m, int s) {
    hour = h;
    minute = m;
    second = s;
  }
  void set(int s) {
    hour = s / 3600;
    minute = s % 3600 / 60;
    second = s % 60;
  }
  int get() { return hour * 3600 + minute * 60 + second; }
  void tick() {
    second++;
    if (second < 60) return;
    second = 0;
    minute++;
    if (minute < 60) return;
    minute = 0;
    hour++;
    if (hour < 24) return;
    hour = 0;
  }
  Time operator+(int s) {
    Time t;
    t.set(get() + s);
    return t;
  }
  friend Time operator+(int s, Time& t) { return t + s; }
  friend ostream& operator<<(ostream& out, Time t) {
    char buf[9];
    sprintf(buf, "%02d:%02d:%02d", t.hour, t.minute, t.second);
    out << buf;
    return out;
  }
  Time& operator++() {
    tick();
    return *this;
  }
  Time operator++(int) {
    Time t = *this;
    tick();
    return t;
  }
};

int main() {
  Time t;
  t.set(23, 59, 59);
  cout << t << endl;
  cout << t + 1 << endl;
  cout << 1 + t << endl;
  cout << ++t << endl;
  cout << t++ << endl;
  cout << t << endl;
  return 0;
}