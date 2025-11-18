/**
 * 在 UML 中，类之间常见的关系有泛化、组合、聚合、关联和依赖。
 * 请根据以下描述确定类之间的关系。并回答在C++中如何实现这些关系。
 * （1）储户与银行账户：一个储户拥有多个银行账户；一个账户只属于一个储户。
 * （2）储户与紧急联系人：每个储户都有一个联系人。
 * （3）ATM 机与银行卡：ATM 机可以存钱与取款，需要插入银行卡并输入密码。
 * （4）个人银行账户与Ⅰ类银行账户、Ⅱ类银行账户、Ⅲ类银行账户：
 * 个人银行账户可以分为Ⅰ类银行账户、Ⅱ类银行账户和Ⅲ类银行账户；三种账户都可以存款取款，
 * 但是它们在限额消费上限、余额上限以及购买金融产品等方面有差异。
 * （5）银行与银行支行：银行支行是银行的分支机构。
 */

/**
 * （1）组合关系
 * （2）关联关系
 * （3）依赖关系
 * （4）泛化关系
 * （5）聚合关系
 */

#include <string>
#include <vector>
using namespace std;

class EmergencyContact {};
class BankAccount {};
class Depositor {
 protected:
  vector<BankAccount> accounts;
  EmergencyContact* emergencyContact;
};
class Card {};
class ATM {
 public:
  void deposit(Card& card, string password, double amount){};
  void withdraw(Card& card, string password, double amount){};
};
class PersonalBankAccount : public BankAccount {
 protected:
  double consumptionLimit;
  double balanceLimit;
  int financialProductsLevel;
};
class Type1BankAccount : public PersonalBankAccount {};
class Type2BankAccount : public PersonalBankAccount {};
class Type3BankAccount : public PersonalBankAccount {};
class BankBranch {};
class Bank {
 protected:
  vector<BankBranch*> branches;
};

int main(){};