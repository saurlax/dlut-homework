/**
 * 编写一个User类，定义2个数据成员：用户id号和密码（必须是6位数字）；
 * 编写一个构造函数，根据id号构造一个新用户（不能与已有id重复），其密码初始化为000000；
 * 编写一个输出id号函数；
 * 写一个修改密码的函数，需要旧密码和新密码。
 * 只有输入的旧密码正确，此功能才能修改密码并提示操作是否成功。
 * 在主函数中测试该User类
 */

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class User {
  static vector<int> ids;
  int id;
  string password;

 public:
  User(int id) {
    if (find(ids.begin(), ids.end(), id) != ids.end()) {
      cout << "Id already exists" << endl;
      return;
    }
    ids.push_back(id);
    this->id = id;
    password = "000000";
  }
  void printId() { cout << id << endl; }
  void changePassword(string oldpwd, string newpwd) {
    if (oldpwd == password) {
      password = newpwd;
      cout << "Password changed successfully" << endl;
    } else {
      cout << "Old password is incorrect" << endl;
    }
  }
};

vector<int> User::ids;

int main() {
  User user1(123), user2(123);
  user1.printId();
  user1.changePassword("000000", "123456");
}