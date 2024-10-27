/**
 * 完成联系人Contact类。联系人Contact类拥有以下属性：
 * 姓名（string）、性别（char）以及联系电话（string）。
 * 根据以下主函数完成Contact类。
 * 注意：联系人的性别只能是M/F/m/f，如果是其他字符则setSex()函数抛出异常；
 * 联系人的电话应是8位0到9的数字，如格式不符合设定要求则setPhone()函数抛出异常。
 */

#include <iostream>
#include <string>
using namespace std;

class Contact {
 public:
  Contact(string name) : name(name) {}
  void setSex(char s) {
    s = tolower(s);
    if (s != 'm' && s != 'f') throw s;
    this->sex = s;
  }
  void setPhone(string p) {
    if (p.size() != 8) throw p;
    for (auto c : p) {
      if (!isdigit(c)) throw p;
    }
    this->phone = p;
  }
  void show() {
    printf("Name: %s\nSex: %c\nPhone: %s\n", name.c_str(), sex, phone.c_str());
  }

 private:
  string name;
  char sex;
  string phone;
};

int main() {
  string name;
  char sex;
  string phone;
  cin >> name >> sex >> phone;
  Contact c1(name);  // 创建联系人Contact的对象c1

  try {
    c1.setPhone(phone);
    c1.setSex(sex);
    c1.show();
  } catch (char sex) {
    cout << "性别设定有误:" << sex << endl;
  } catch (string& phone) {
    cout << "电话号码设定有误:" << phone << endl;
  }
}