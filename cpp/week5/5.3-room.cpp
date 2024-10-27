/**
 * 根据房间 Room 类设计完成教室 ClassRoom 类。
 * ClassRoom 类是 Room 类的派生类，具有 seat 属性，代表教室座位数；
 * 还具有函数 calAverArea()，可以计算得到每个座位所占的平均面积。
 * 房间 Room 类与主函数定义如下。

程序运行结果如下：
创建房间：12 米, 10 米, 30 个座位
长 12 米, 宽 10 米, 30 个座位, 平均面积 4 平米
*/

#include <iostream>
using namespace std;
class Room {
 protected:
  double length;
  double width;

 public:
  Room(double l, double w) : length(l), width(w) {}
};

class ClassRoom : public Room {
  int seat;

 public:
  ClassRoom(int s, double l, double w) : Room(l, w), seat(s) {}
  double calAverArea() { return length * width / seat; }
  void show() {
    cout << "创建房间：" << length << " 米, " << width << " 米, " << seat
         << " 个座位" << endl;
    cout << "长 " << length << " 米, 宽 " << width << " 米, " << seat
         << " 个座位, 平均面积 " << calAverArea() << " 平米" << endl;
  }
};

int main() {
  ClassRoom cr(30, 12, 10);
  cr.show();
  return 0;
}