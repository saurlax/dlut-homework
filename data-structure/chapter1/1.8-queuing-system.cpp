/**
 * 银行叫号系统
 */

#include <iostream>
#include <queue>
#include <string>
using namespace std;

class BankQueue {
  int number = 0;
  queue<pair<int, string>> q;

public:
  int take_number(string name) {
    q.push({++number, name});
    return number;
  }

  pair<int, string> call_number() {
    if (q.empty()) {
      cout << "No number!" << endl;
      return {-1, ""};
    }
    auto p = q.front();
    q.pop();
    return p;
  }
};

int main() {
  BankQueue bank;
  cout << "Welcome to the bank!" << endl;
  while (true) {
    int op;
    cout << "1. Take a number" << endl;
    cout << "2. Call a number" << endl;
    cout << "3. Exit" << endl;
    cin >> op;
    if (op == 1) {
      string name;
      cout << "Please input your name: ";
      cin >> name;
      cout << "Your number is: " << bank.take_number(name) << endl;
    } else if (op == 2) {
      auto p = bank.call_number();
      if (p.first != -1)
        cout << "Number: " << p.first << " Name: " << p.second << endl;
    } else if (op == 3) {
      break;
    } else {
      cout << "Invalid operation!" << endl;
    }
  }
  return 0;
}