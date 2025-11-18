/**
 * 定义一个时间类Time,它能提供由时分秒组成的时间。要求能够设置时间和打印出时间。
 * 数据成员：时、分、秒
 * 成员函数：
 * 若干set函数，可以设置时间各个组成部分，也可以整体设置时间;
 * 可以只设置秒，时和分为0
 *
 * 有一个tick函数，负责给当前时间加1秒钟，要保证测试下列情况：
 *    a)递增到下一分钟。
 *    b)递增到下一小时。
 *    c)递增到下一天(即11:59:59PM到00:00:00AM)。
 *
 * 有一个打印输出函数
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

  void set(int s) { set(0, 0, s); }

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

  void print() { printf("%02d:%02d:%02d\n", hour, minute, second); }
};

int main() {
  Time t;
  t.set(23, 59, 59);
  t.tick();
  t.print();
  return 0;
}