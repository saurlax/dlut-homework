/**
 * 用面向对象方法设计并完成一个计算器控制台程序。
 * （1）设计一个父类Operation, 数据成员包含两个操作数m_numberA，m_numberB。
 * 成员函数包括set(),get()函数设置操作数的值，虚函数GetResult()用于输出结果。
 * （2）分别设计派生类 OperationAdd，OperationSub，OperationMul，
 * OperationDiv实现加减乘除操作,注意除法操作除数不能为0。
 * （3）派生类统一继承自父类Operation, 重写虚函数GetResult()输出操作后的结果。
 * （4）OperationFactory类已经写好，可以直接使用。
 */

#include <iostream>
using namespace std;

class Operation {
 public:
  void SetNumA(double numA) { m_numberA = numA; }
  void SetNumB(double numB) { m_numberB = numB; }
  double GetNumA() { return m_numberA; }
  double GetNumB() { return m_numberB; }
  virtual double GetResult() { return 0; }

 private:
  double m_numberA;
  double m_numberB;
};

class OperationAdd : public Operation {
 public:
  double GetResult() { return GetNumA() + GetNumB(); }
};

class OperationSub : public Operation {
 public:
  double GetResult() { return GetNumA() - GetNumB(); }
};

class OperationMul : public Operation {
 public:
  double GetResult() { return GetNumA() * GetNumB(); }
};

class OperationDiv : public Operation {
 public:
  double GetResult() {
    if (GetNumB() == 0) {
      cout << "NumB cannot be zero!" << endl;
      return 0;
    }
    return GetNumA() / GetNumB();
  }
};

class OperationFactory {
 public:
  static Operation* CreateOperate(char oper) {
    Operation* poper = NULL;
    switch (oper) {
      case '+':
        poper = new OperationAdd;
        break;
      case '-':
        poper = new OperationSub;
        break;
      case '*':
        poper = new OperationMul;
        break;
      case '/':
        poper = new OperationDiv;
        break;
    }
    return poper;
  }
};
int main() {
  int flag = 1;
  while (flag) {
    cout << "input A:";
    double numA;
    cin >> numA;
    cout << "input operator:";
    char oper;
    cin >> oper;
    cout << "input B:";
    double numB;
    cin >> numB;
    Operation* poper = OperationFactory::CreateOperate(oper);
    poper->SetNumA(numA);
    poper->SetNumB(numB);
    cout << poper->GetNumA() << ' ' << oper << ' ' << poper->GetNumB() << " = "
         << poper->GetResult() << endl;
    cout << "是否需要继续: (0/1)";
    cin >> flag;
  }
  return 0;
}
