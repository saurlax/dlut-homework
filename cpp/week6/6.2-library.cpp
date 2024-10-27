/**
 * 利用面向对象的思想为“图书馆”设计一个图书管理程序。
 * 图书馆馆藏（Publication）可分为图书（Book）类、报纸（Newspaper）类和杂志（Magzine）类。
 * 所有馆藏都应保存它们的名称（书名、报纸名等）。
 * 此外图书应保存图书的作者信息；报纸需要保存它的发行时间；杂志需要保存它的发行时间和卷号。要求：
 * （1）设计出所有的类，只需要写出类最主要属性（本题中涉及到的）和函数。
 * （2）完成图书馆类Library。类的声明如下：
 * 在主函数中构造若干出版物（图书、报纸、杂志），将他们添加到图书馆，
 * 并使用图书馆的show()函数输出信息。观察程序运行结果。
 */

#include <iostream>
#include <string>
using namespace std;

class Publication {
 protected:
  string name;

 public:
  Publication(string name) : name(name) {}
  virtual void show() { cout << name << endl; }
};

class Book : public Publication {
 protected:
  string author;

 public:
  Book(string name, string author) : Publication(name), author(author) {}
  void show() { cout << name << " " << author << endl; }
};

class Newspaper : public Publication {
 protected:
  string issueTime;

 public:
  Newspaper(string name, string issueTime)
      : Publication(name), issueTime(issueTime) {}
  void show() { cout << name << " " << issueTime << endl; }
};

class Magazine : public Publication {
 protected:
  string issueTime;
  string volume;

 public:
  Magazine(string name, string issueTime, string volume)
      : Publication(name), issueTime(issueTime), volume(volume) {}
  void show() { cout << name << " " << issueTime << " " << volume << endl; }
};

class Library {
  Publication* publications[20];  // 所有馆藏，假设馆藏上限为20
  static int total;               // 实际馆藏数
 public:
  void add(Publication& p) { publications[total++] = &p; };  // 添加新馆藏
  void show() {  // 输出所有馆藏的相应信息
    for (int i = 0; i < total; i++) publications[i]->show();
  }
};

int Library::total = 0;

int main() {
  Library lib;
  Book book("C++", "Nan Jiang");
  Newspaper newspaper("People's Daily", "2023/1/1");
  Magazine magazine("Computer Science", "2023/5/1", "volume18");
  lib.add(book);
  lib.add(newspaper);
  lib.add(magazine);
  lib.show();
  return 0;
}