/**
 * 店铺配送一种商品，按重量装箱买卖，每箱重量和价格各不相同。
 * 因此，店铺需要记录当前库存商品的“总重量”和“总价值”。
 * 编写一个程序，通过定义“Carlo”类来模拟商店中商品的购买和销售。
 * 该任务训练使用静态数据成员。
 * 定义私有变量存储每件商品的价格和重量，使用静态数据成员存储商品的总重量和总价；
 * 定义构造函数和析构函数。
 * 当一个新对象的定义和初始化完成或对象被删除时，在总重量和总价格中加上或减去相应对象的重量和价格。
 */

#include <iostream>
using namespace std;

class Carlo {
  double weight;
  double price;

 public:
  static double totalWeight;
  static double totalPrice;

  Carlo(double w, double p) : weight(w), price(p) {
    totalWeight += weight;
    totalPrice += price;
  }

  ~Carlo() {
    totalWeight -= weight;
    totalPrice -= price;
  }
};

double Carlo::totalWeight = 0;
double Carlo::totalPrice = 0;

int main() {
  Carlo a(1, 2);
  Carlo b(2, 3);
  Carlo *c = new Carlo(3, 4);
  cout << "Total weight: " << Carlo::totalWeight << endl;
  cout << "Total price: " << Carlo::totalPrice << endl;
  delete c;
  cout << "Total weight: " << Carlo::totalWeight << endl;
  cout << "Total price: " << Carlo::totalPrice << endl;
  return 0;
}