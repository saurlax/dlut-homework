/**
 * （1）定义普通函数“double pointDistance (Point &p1, Point &p2)”
 * 计算两点之间的距离，并将该函数声明为“Point”类的友元函数。
 * （2）定义“Distance”类，
 * 其成员函数为“double pointDistance (Point &p1, Point &p2)”。
 * 将“Distance”类声明为“Point”类的友元类。
 * （3）定义“Distance”类，
 * “double pointDistance (Point &p1, Point &p2)”是该类的成员函数，
 * 声明为“Point”类的友元函数。
 */

#include <cmath>
#include <iostream>
using namespace std;

class Point;
class Distance {
 public:
  double pointDistance(Point &p1, Point &p2);
};

class Point {
  double x, y;

 public:
  Point(double x = 0, double y = 0) : x(x), y(y) {}
  friend double pointDistance(Point &p1, Point &p2);
  friend class Distance;
  friend double Distance::pointDistance(Point &p1, Point &p2);
};

double pointDistance(Point &p1, Point &p2) {
  return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

double Distance::pointDistance(Point &p1, Point &p2) {
  return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

int main() {
  Point p1(1, 2), p2(3, 4);
  cout << pointDistance(p1, p2) << endl;
  Distance d;
  cout << d.pointDistance(p1, p2) << endl;
  return 0;
}