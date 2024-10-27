/**
 * 实现图 5.3 中的类以及类关系。雇员有姓名和员工号；
 * 其他类可根据需要加入必要的属性与函数。
 * 在主函数中对编写的所有类进行测试，注意考虑虚基类的使用。
 */

#include <iostream>
#include <string>
using namespace std;

class Employee {
 protected:
  string name;
  int id;

 public:
  Employee(string n, int i) : name(n), id(i) {}
  friend ostream& operator<<(ostream& os, const Employee& e) {
    os << "姓名：" << e.name << "，员工号：" << e.id;
    return os;
  }
};

class Technician : virtual public Employee {
 protected:
  int ability;

 public:
  Technician(string n, int i, int l) : Employee(n, i), ability(l) {}
  friend ostream& operator<<(ostream& os, const Technician& t) {
    os << "姓名：" << t.name << "，员工号：" << t.id << "，技术级别："
       << t.ability;
    return os;
  }
};

class Manager : virtual public Employee {
 protected:
  int level;

 public:
  Manager(string n, int i, int l) : Employee(n, i), level(l) {}
  friend ostream& operator<<(ostream& os, const Manager& m) {
    os << "姓名：" << m.name << "，员工号：" << m.id << "，管理等级："
       << m.level;
    return os;
  }
};

class Salesman : virtual public Employee {
 protected:
  double sales;

 public:
  Salesman(string n, int i, double s) : Employee(n, i), sales(s) {}
  friend ostream& operator<<(ostream& os, const Salesman& s) {
    os << "姓名：" << s.name << "，员工号：" << s.id << "，销售额：" << s.sales;
    return os;
  }
};

class TechnicianManager : public Technician, public Manager {
 public:
  TechnicianManager(string n, int i, int l1, int l2)
      : Employee(n, i), Technician(n, i, l1), Manager(n, i, l2) {}
  friend ostream& operator<<(ostream& os, const TechnicianManager& tm) {
    os << "姓名：" << tm.name << "，员工号：" << tm.id << "，技术级别："
       << tm.ability << "，管理等级：" << tm.level;
    return os;
  }
};

class SalesmanManager : public Salesman, public Manager {
 public:
  SalesmanManager(string n, int i, double s, int l)
      : Employee(n, i), Salesman(n, i, s), Manager(n, i, l) {}
  friend ostream& operator<<(ostream& os, const SalesmanManager& sm) {
    os << "姓名：" << sm.name << "，员工号：" << sm.id << "，销售额："
       << sm.sales << "，管理等级：" << sm.level;
    return os;
  }
};

int main() {
  Employee e("张三", 1001);
  Technician t("李四", 1002, 3);
  Manager m("王五", 1003, 2);
  Salesman s("赵六", 1004, 10000);
  TechnicianManager tm("孙七", 1005, 4, 3);
  SalesmanManager sm("周八", 1006, 20000, 2);
  cout << e << endl;
  cout << t << endl;
  cout << m << endl;
  cout << s << endl;
  cout << tm << endl;
  cout << sm << endl;
  return 0;
}