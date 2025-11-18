#include <iostream>
#include <iomanip>
#include <malloc.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>

class DiskArm
{
public:
  DiskArm(int currentCylinder, int seekDirection, int requestNumber, int* requests);
  ~DiskArm();
  void initSpace(const char *methodName); // 初始化寻道记录
  void report();                    // 报告算法执⾏情况
  void fcfs();
  void sstf();
  void scan();
  void cscan();
  void look();

private:
  int *request;        // 磁盘请求道号
  int *cylinder;       // ⼯作柱⾯道号
  int requestNumber;   // 磁盘请求数
  int currentCylinder; // 当前道号
  int seekDirection;   // 磁头⽅向
  int seekNumber;      // 移臂总数
  int seekChange;      // 磁头调头数
  int maxCylinder;     // 最大柱面号
};