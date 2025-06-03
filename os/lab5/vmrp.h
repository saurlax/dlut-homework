#include <iostream>
#include <iomanip>
#include <malloc.h>
#include <cstdlib>
#include <ctime>

class Replace
{
public:
  Replace();
  ~Replace();
  void initSpace(char *methodName); // 初始化页号记录
  void report(void);                // 报告算法执行情况
  void fifo(void);                  // 先进先出算法
  void lru(void);                   // 最近最少未用算法
  void clock(void);                 // 时钟(二次机会)置换算法
  void eclock(void);                // 增强二次机会置换算法
  void lfu(void);                   // 最不经常使用置换算法
  void mfu(void);                   // 最经常使用置换算法
  void generateRandomSequence(int pageRange); // 随机生成页面引用串
  void generateBelady(void);        // 生成Belady测试序列
  void resetData(void);             // 重置数据以便重复测试
  void compareAlgorithms(void);     // 比较所有算法性能

private:
  int *referencePage; // 存放要访问到的页号
  int *eliminatePage; // 存放淘汰页号
  int *pageFrames;    // 存放当前正在实存中的页号
  int pageNumber;     // 访问页数
  int frameNumber;    // 实存帧数
  int faultNumber;    // 失败页数
};