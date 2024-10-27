/**
 * 按要求完成短信类的实现，使得main函数正常运行，main函数不得修改， Message类结构不得修改。
 * 1. 开发环境visual studio2022; 也可用其他环境（删掉#pragme一行）；
 * 2. send()函数一般返回发送的字符串到短消息网关，但在本例中，该函数中直接输出相关信息；类的其他函数不做输入输出；
 * 3. 本例重点1要搞清楚本例字符串成员的两种存储方式；
 * 4. 本例重点2要搞清楚构造函数、拷贝构造函数以及析构函数。
*/

#include <cstring>
#include <ctime>
#include <iostream>
using namespace std;

// 类结构不要更改
class Message {  // 短消息类
 private:
  char* body;            // 消息正文
  char from[20];         // 发送者
  char to[20];           // 接受者
  static int sucessCnt;  // 成功发送消息数量
 public:
  // 参数为消息正文、发送者、接受者;若参数b为空指针，body为空指针；
  // 若参数f、t为空指针，from、to为空串（即字符数组开头字符为\0）；
  Message(const char* b = NULL, const char* f = NULL, const char* t = NULL);
  Message(const Message& m);
  ~Message();
  // 发送短消息，消息正文(不空指针并且不空串)、发送者（不空串）、接受者（不空串）
  // 都不空时发送成功返回1，失败返回0
  int send();  // 此函数里面允许把发送内容直接写输出。
  static int getSucessCnt();  // 获得成功发送消息数量
};
// 完成Message类的实现

Message::Message(const char* b, const char* f, const char* t) {
  if (b == NULL) {
    body = NULL;
  } else {
    body = new char[strlen(b) + 1];
    strcpy(body, b);
  }
  if (f == NULL) {
    from[0] = '\0';
  } else {
    strcpy(from, f);
  }
  if (t == NULL) {
    to[0] = '\0';
  } else {
    strcpy(to, t);
  }
}

Message::Message(const Message& m) {
  if (m.body == NULL) {
    body = NULL;
  } else {
    body = new char[strlen(m.body) + 1];
    strcpy(body, m.body);
  }
  strcpy(from, m.from);
  strcpy(to, m.to);
}

Message::~Message() {
  if (body != NULL) {
    delete[] body;
  }
}

int Message::send() {
  if (body == NULL || from[0] == '\0' || to[0] == '\0') {
    return 0;
  }
  cout << from << " 发送给 " << to << ": " << body << endl;
  sucessCnt++;
  return 1;
}

int Message::getSucessCnt() { return sucessCnt; }

int Message::sucessCnt = 0;

// main函数不要更改。
int main() {
  Message ms1("hello", "zhangsan", "lisi");
  if (ms1.send()) cout << "发送成功" << endl;

  Message ms2("hello");
  if (ms2.send())
    cout << "发送成功" << endl;
  else
    cout << "发送失败" << endl;

  Message ms3 = ms1;
  if (ms3.send())
    cout << "发送成功" << endl;
  else
    cout << "发送失败" << endl;

  cout << "发送成功消息数量" << Message::getSucessCnt() << endl;
}
