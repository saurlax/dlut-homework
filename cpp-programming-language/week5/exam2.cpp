/**
 * 完成本科类、研究生类、教师类、助教类、课程类以及学生列表类的设计。
 * 简述类的关系，完成类的框架以体现类的结构与关系。
 * （1）课程有名称与课程号。
 * （2）本科类有身份号码、姓名、学号、专业。
 * 研究生有身份号码、姓名、学号、专业、研究方向。
 * 教师数据有身份号码，姓名、教工号、若干授课课程。
 * 要求运用继承的思想设计本科生、研究生和教师类（不需要非常完备的功能）。
 * 这些类都具有show()函数用来显示各自的信息。
 * （3）部分研究生可以担任助教，除了研究生的数据之外还具有编号（助教专用编号）。
 * 每个助教可以为一位老师的一门课程助课（只能一门课程），助教可以批作业。
 * （4）学生列表包含所有学生，具有show()函数，
 * 该函数运行则列表中所有学生均可调用其show()函数显示各自信息：
 * 输出本科生以及研究生的基本信息，
 * 担任助教的研究生可以显示助课信息（为哪位老师助课，课程名）。
 * （5）在主函数测试本科生、研究生、教师、课程以及助教类。
 * 为助教指派助课任务后，教师可以让助教批作业。
 * 通过创建学生列表显示所有学生信息。
 * 本题主要考察继承、多态以及类的关系。
 */

/**
 * 本科生类和研究生类都是学生类的派生类；
 * 学生类、教师类和助教类都是人类的派生类；
 * 学生类是学生列表类的成员。
 */

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Course;

class Person {
 public:
  Person(string id, string name) : id(id), name(name) {}
  virtual void show();

 protected:
  string id, name;
};

class Student : public Person {
 public:
  Student(string id, string name, string studentId, string major)
      : Person(id, name), studentId(studentId), major(major) {}
  void show();

 protected:
  string studentId, major;
};

class Undergraduate : public Student {
 public:
  Undergraduate(string id, string name, string studentId, string major)
      : Student(id, name, studentId, major) {}
};

class Graduate : public Student {
 public:
  Graduate(string id, string name, string studentId, string major,
           string researchDirection)
      : Student(id, name, studentId, major),
        researchDirection(researchDirection) {}
  void show();

 protected:
  string researchDirection;
};

class TA : public Graduate {
 public:
  TA(string id, string name, string studentId, string major,
     string researchDirection, string taId)
      : Graduate(id, name, studentId, major, researchDirection),
        taId(taId),
        course(course) {}
  void show();
  void assignCourse(Course *course);
  void correctHomework();

 protected:
  string taId;
  Course *course;
};

class StudentList {
 public:
  void push(Student *student);
  void show();

 protected:
  vector<Student *> students;
};

class Teacher : public Person {
 public:
  Teacher(string id, string name, string teacherId)
      : Person(id, name), teacherId(teacherId) {}
  void show();
  void correctHomework();

 protected:
  string teacherId;
  vector<Course> courses;
};

class Course {
 public:
  Course(string name, string courseId, StudentList *students, Teacher *teacher,
         TA *ta)
      : name(name),
        courseId(courseId),
        students(students),
        teacher(teacher),
        ta(ta) {}
  void show();
  void assignTA(TA *ta);
  void correctHomework();

 protected:
  string name, courseId;
  StudentList *students;
  Teacher *teacher;
  TA *ta;
};

void Person::show() { printf("ID: %s, Name: %s\n", id.c_str(), name.c_str()); }

void Graduate::show() {
  printf(
      "ID: %s, Name: %s, Student ID: %s, Major: %s, Research Direction: %s\n",
      id.c_str(), name.c_str(), studentId.c_str(), major.c_str(),
      researchDirection.c_str());
}

void Student::show() {
  printf("ID: %s, Name: %s, Student ID: %s, Major: %s\n", id.c_str(),
         name.c_str(), studentId.c_str(), major.c_str());
}

void TA::show() {
  printf(
      "ID: %s, Name: %s, Student ID: %s, Major: %s, Research Direction: %s, "
      "TA ID: %s\n",
      id.c_str(), name.c_str(), studentId.c_str(), major.c_str(),
      researchDirection.c_str(), taId.c_str());
  course->show();
}

void TA::assignCourse(Course *course) { this->course = course; }

void TA::correctHomework() { printf("Correcting homework.\n"); }

void StudentList::push(Student *student) { students.push_back(student); }

void StudentList::show() {
  for (int i = 0; i < students.size(); i++) students[i]->show();
}

void Teacher::show() {
  printf("ID: %s, Name: %s, Teacher ID: %s\n", id.c_str(), name.c_str(),
         teacherId.c_str());
}

void Teacher::correctHomework() {
  for (int i = 0; i < courses.size(); i++) courses[i].correctHomework();
}

void Course::show() {
  printf("Course: %s, Course ID: %s\n", name.c_str(), courseId.c_str());
}

void Course::assignTA(TA *ta) {
  this->ta = ta;
  ta->assignCourse(this);
}

void Course::correctHomework() {
  if (ta) ta->correctHomework();
}

int main() {
  Undergraduate undergraduate("1", "Tom", "1001", "Computer Science");
  Graduate graduate("2", "Jerry", "2001", "Computer Science", "AI");
  StudentList studentList;
  studentList.push(&undergraduate);
  studentList.push(&graduate);
  studentList.show();
  Teacher teacher("3", "Alice", "3001");
  TA ta("4", "Bob", "2002", "Computer Science", "AI", "4001");
  Course course("AI", "5001", &studentList, &teacher, &ta);
  course.assignTA(&ta);
  ta.show();
  teacher.correctHomework();
  return 0;
}