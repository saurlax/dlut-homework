/**
 * 某公司有四种雇员：高级雇员、中级雇员、初级雇员以及临时雇员。
 * 雇员每月的薪酬计算方式为固定工资+计时工资。
 * 高级雇员的固定工资为每月5000元；中级雇员的固定工资为每月4000元；
 * 初级雇员的固定工资为每月3000元；临时雇员没有固定工资。
 * 计时工资每小时30元，高级雇员、中级雇员和初级雇员都可以根据本月累计的额外的计时时长计算计时工资。
 * 临时雇员的工资全部来自计时工资。
 * （1）根据描述完成所有雇员类的设计。只需写出类最主要的属性（涉及到的）和函数。
 * （2）在主函数中创建若干雇员并录入所有雇员某月计时工作的时常，并测试薪酬的计算函数。
 * （3）在主函数中用“雇员数组”的方式管理所有雇员，并统一输出雇员的薪酬信息。
 */

#include <iostream>
#include <string>
using namespace std;

class Employee {
 public:
  Employee(double worktime = 0) : worktime(worktime){};
  double getSalary() { return fixedSalary + hourlySalary * worktime; }

 protected:
  double fixedSalary = 0;
  double hourlySalary = 30;
  double worktime;
};

class SeniorEmployee : public Employee {
 public:
  SeniorEmployee(double worktime = 0) : Employee(worktime) {
    fixedSalary = 5000;
  }
};

class MiddleEmployee : public Employee {
 public:
  MiddleEmployee(double worktime = 0) : Employee(worktime) {
    fixedSalary = 4000;
  }
};

class JuniorEmployee : public Employee {
 public:
  JuniorEmployee(double worktime = 0) : Employee(worktime) {
    fixedSalary = 3000;
  }
};

class TempEmployee : public Employee {
 public:
  TempEmployee(double worktime = 0) : Employee(worktime) {}
};

int main() {
  Employee employees[4] = {SeniorEmployee(10), MiddleEmployee(20),
                           JuniorEmployee(30), TempEmployee(40)};
  for (int i = 0; i < 4; i++) {
    cout << "Salary: " << employees[i].getSalary() << endl;
  }
  return 0;
}