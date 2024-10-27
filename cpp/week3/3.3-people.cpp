/**
 * 设计一个“People”类进行人事管理，属性有：
 * “number”（编号）、“name”（姓名，为char[]或string类型）、
 * “sex”（性别）、“birthday”（出生日期）、“id”（身份证号码）等。
 * “生日”被定义为内嵌日期对象（类的组合）。
 * 使用成员函数输入和显示人员信息。
 * 注意：此任务需要先完成“日期”类。 注意日期的合理性，包括闰年的判断。
 */

#include <iostream>
#include <string>
using namespace std;

class Date {
  int year, month, day;

 public:
  Date(int y = 0, int m = 0, int d = 0) { set(y, m, d); }
  void set(int y, int m, int d) {
    bool valid = true;
    if (valid) valid = m >= 1 && m <= 12;
    if (valid && m == 2) {
      if (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0)) {
        valid = d <= 29;
      } else {
        valid = d <= 28;
      }
    }
    if (valid && (m == 4 || m == 6 || m == 9 || m == 11)) {
      valid = d <= 30;
    } else {
      valid = d <= 31;
    }
    if (!valid) year = 2000, month = 1, day = 1;
    year = y, month = m, day = d;
  }
  friend ostream& operator<<(ostream& os, const Date& d) {
    os << d.year << "-" << d.month << "-" << d.day;
    return os;
  }
};

class People {
  int number;
  string name;
  bool sex;
  Date birthday;
  string id;

 public:
  People(int num, string name, bool s, Date b, string i)
      : number(num), name(name), sex(s), birthday(b), id(i) {}
  void show() {
    cout << "Number: " << number << endl;
    cout << "Name: " << name << endl;
    cout << "Sex: " << (sex ? "Male" : "Female") << endl;
    cout << "Birthday: " << birthday << endl;
    cout << "ID: " << id << endl;
  }
};

int main() {
  People p(1, "Tom", true, Date(2000, 1, 1), "1234567890");
  p.show();
  return 0;
}