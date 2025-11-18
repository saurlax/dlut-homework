/**
 * (1) 有 “Point” 类与主函数如下（该任务训练对象数组）:
 * void main() {
 *   Point p[3];
 *   p[1].set(1.0,2.0);
 *   p[2].set(3.0,4.0);
 *   p[1].show();
 * }
 * 程序输出如下
 * Create point(0,0).
 * Create point(0,0).
 * Create point(0,0).
 * point is (1.0,2.0).
 * 请完成 “Point” 类. (x与y坐标都是float)
 *
 * (2) 基于“Point”类，设计并定义一个“Rectangle”类。
 * 这个类的数据成员是矩形的左下角和右上角的坐标。
 * 矩形可以计算自己的面积。
 * 两个点的坐标可以在矩形初始化的时候初始化，也可以通过函数来重新设置。
 * 请采用类的组合。
 *
 * (3) 定义并实现一个“PointSet”类（点的集合）。这个类是一个集合。
 * 可以将不超过5个点放入集合中或将它们从集合中取出。
 * 也可以对所有点成员进行统一处理（将所有点同时设置）。
 * 集合超过 5 个元素后，不能再添加元素。
 */

#include <iomanip>
#include <iostream>
using namespace std;

class Point {
  float x, y;

 public:
  Point(float x = 0, float y = 0) : x(x), y(y) {
    cout << "Create point(" << x << "," << y << ")." << endl;
  }
  void set(float a, float b) {
    x = a;
    y = b;
  }
  void show() {
    cout << fixed << setprecision(1) << "point is (" << x << "," << y << ")."
         << endl;
  }
  friend class Rectangle;
  friend class PointSet;
};

class Rectangle {
  Point p1, p2;

 public:
  Rectangle(Point p1 = Point(0, 0), Point p2 = Point(0, 0)) { set(p1, p2); }
  void set(Point p1, Point p2) {
    if (p1.x > p2.x || p1.y > p2.y) {
      cout << "Invalid rectangle." << endl;
      p1.set(0, 0);
      p2.set(0, 0);
    }
    this->p1 = p1;
    this->p2 = p2;
  }
  float area() { return (p2.x - p1.x) * (p2.y - p1.y); }
};

class PointSet {
  Point p[5];
  int count;

 public:
  PointSet() : count(0) {}
  void add(Point point) {
    if (count < 5) {
      p[count++] = point;
    } else {
      cout << "PointSet is full." << endl;
    }
  }
  void remove(Point point) {
    for (int i = 0; i < count; i++) {
      if (p[i].x == point.x && p[i].y == point.y) {
        for (int j = i; j < count - 1; j++) {
          p[j] = p[j + 1];
        }
        count--;
        break;
      }
    }
  }
  void setAll(float x, float y) {
    for (int i = 0; i < count; i++) {
      p[i].set(x, y);
    }
  }
};

int main() {
  Point p[3];
  p[1].set(1.0, 2.0);
  p[2].set(3.0, 4.0);
  p[1].show();

  Rectangle r(p[1], p[2]);
  cout << "Area of rectangle: " << r.area() << endl;

  PointSet ps;
  ps.add(p[1]);
  ps.add(p[2]);
  ps.add(Point(5, 6));
  ps.add(Point(7, 8));
  ps.add(Point(9, 10));
  ps.add(Point(11, 12));
  ps.remove(p[1]);
  ps.setAll(0, 0);
  return 0;
}