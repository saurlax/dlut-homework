#include "vmrp.h"

Replace::Replace()
{
  int i;
  int inputMode;
  
  std::cout << "请选择输入模式：\n";
  std::cout << "1 - 手动输入页面引用串\n";
  std::cout << "2 - 使用Belady测试序列\n";
  std::cout << "3 - 随机生成页面引用串\n";
  std::cout << "请选择（1-3）：";
  std::cin >> inputMode;
  
  // 设定总的访问页数，并分配相应的引用页号和淘汰页号记录数组空间
  std::cout << "请输入页面数量：";
  std::cin >> pageNumber;
  referencePage = new int[pageNumber];
  eliminatePage = new int[pageNumber];
  
  if (inputMode == 1) {
    // 手工输入引用页号序列
    std::cout << "请输入页面引用串：";
    for (i = 0; i < pageNumber; i++)
      std::cin >> referencePage[i];
  } else if (inputMode == 2) {
    // Belady序列
    generateBelady();
  } else if (inputMode == 3) {
    // 随机生成页面引用串
    int pageRange;
    std::cout << "请输入页面范围（如输入10表示页面号范围0-9）：";
    std::cin >> pageRange;
    generateRandomSequence(pageRange);
  }
  
  // 设定内存实页数(帧数)
  std::cout << "请输入内存帧数：";
  std::cin >> frameNumber;
  pageFrames = new int[frameNumber];
}

Replace::~Replace()
{
  delete[] referencePage;
  delete[] eliminatePage;
  delete[] pageFrames;
}

void Replace::initSpace(char *methodName)
{
  int i;
  std::cout << "\n" << methodName << "\n";
  faultNumber = 0;
  // 引用还未开始，-1表示无引用页
  for (i = 0; i < pageNumber; i++)
    eliminatePage[i] = -1;
  for (i = 0; i < frameNumber; i++)
    pageFrames[i] = -1;
}

void Replace::report(void)
{
  // 报告淘汰页顺序
  std::cout << "\n淘汰页序列：";
  for (int i = 0; eliminatePage[i] != -1; i++)
    std::cout << eliminatePage[i] << " ";
  // 报告缺页数和缺页率
  std::cout << "\n缺页次数=" << faultNumber << "\n";
  std::cout << std::setw(6) << std::setprecision(3);
  std::cout << "缺页率=" << 100 * (float)faultNumber / (float)pageNumber << "%\n";
}

void Replace::lru(void)
{
  int i, j, k, next;
  initSpace("LRU");
  // 循环装入引用页
  for (k = 0, i = 0; k < pageNumber; k++)
  {
    next = referencePage[k];
    // 检测引用页当前是否已在实存
    for (i = 0; i < frameNumber; i++)
    {
      if (next == pageFrames[i])
      {
        // 引用页已在实存将其调整到页记录栈顶
        for (j = i; j > 0; j--)
          pageFrames[j] = pageFrames[j - 1];
        pageFrames[0] = next;
        break;
      }
    }
    if (pageFrames[0] == next)
    {
      // 如果引用页已放栈顶，则为不缺页，报告当前内存页号
      for (j = 0; j < frameNumber; j++)
        if (pageFrames[j] >= 0)
          std::cout << pageFrames[j] << " ";
      std::cout << "\n";
      continue; // 继续装入下一页
    }
    else
    {
      // 如果引用页还未放栈顶，则为缺页，缺页数加1
      faultNumber++;
      // 栈底页号记入淘汰页数组中
      eliminatePage[i] = pageFrames[frameNumber - 1];
      // 向下压栈
      for (j = frameNumber - 1; j > 0; j--)
        pageFrames[j] = pageFrames[j - 1];
      pageFrames[0] = next; // 引用页放栈顶
      // 报告当前实存中页号
      for (j = 0; j < frameNumber; j++)
        if (pageFrames[j] >= 0)
          std::cout << pageFrames[j] << " ";
      // 报告当前淘汰的页号
      if (eliminatePage[i] >= 0)
        std::cout << "->" << eliminatePage[i++] << "\n";
      else
        std::cout << "\n";
    }
  }
  report();
}

void Replace::fifo(void)
{
  int i, j, k, next, l = 0;
  initSpace("FIFO");
  // 循环装入引用页
  for (k = 0, j = 0; k < pageNumber; k++)
  {
    next = referencePage[k];
    // 如果引用页已在实存中，报告实存页号
    for (i = 0; i < frameNumber; i++)
      if (next == pageFrames[i])
        break;
    if (i < frameNumber)
    {
      for (i = 0; i < frameNumber; i++)
        std::cout << pageFrames[i] << " ";
      std::cout << "\n";
      continue; // 继续引用下一页
    }
    // 引用页不在实存中，缺页数加1
    faultNumber++;
    eliminatePage[l] = pageFrames[j]; // 最先入页号记入淘汰页数组
    pageFrames[j] = next;             // 引用页号放最先入页号处
    j = (j + 1) % frameNumber;        // 最先入页号循环下移
    // 报告当前实存页号和淘汰页号
    for (i = 0; i < frameNumber; i++)
      if (pageFrames[i] >= 0)
        std::cout << pageFrames[i] << " ";
    if (eliminatePage[l] >= 0)
      std::cout << "->" << eliminatePage[l++] << "\n";
    else
      std::cout << "\n";
  }
  report();
}

void Replace::clock(void)
{
  int i, j, k, next;
  int pointer = 0;                            // 时钟指针
  bool *referenceBit = new bool[frameNumber]; // 引用位数组

  initSpace("CLOCK");

  // 初始化引用位
  for (i = 0; i < frameNumber; i++)
    referenceBit[i] = false;

  // 循环装入引用页
  for (k = 0, j = 0; k < pageNumber; k++)
  {
    next = referencePage[k];

    // 检查页面是否已在内存中
    for (i = 0; i < frameNumber; i++)
    {
      if (next == pageFrames[i])
      {
        referenceBit[i] = true; // 设置引用位
        break;
      }
    }

    if (i < frameNumber)
    {
      // 页面命中，输出当前内存状态
      for (i = 0; i < frameNumber; i++)
        if (pageFrames[i] >= 0)
          std::cout << pageFrames[i] << " ";
      std::cout << "\n";
      continue;
    }

    // 页面缺失
    faultNumber++;

    // 查找可替换的页面
    while (true)
    {
      if (pageFrames[pointer] == -1)
      {
        // 空闲页框
        break;
      }
      if (!referenceBit[pointer])
      {
        // 找到引用位为0的页面
        break;
      }
      // 引用位为1，清零并继续
      referenceBit[pointer] = false;
      pointer = (pointer + 1) % frameNumber;
    }

    // 记录被淘汰的页面
    eliminatePage[j] = pageFrames[pointer];
    pageFrames[pointer] = next;
    referenceBit[pointer] = true;
    pointer = (pointer + 1) % frameNumber;

    // 输出当前状态
    for (i = 0; i < frameNumber; i++)
      if (pageFrames[i] >= 0)
        std::cout << pageFrames[i] << " ";

    if (eliminatePage[j] >= 0)
      std::cout << "->" << eliminatePage[j++] << "\n";
    else
      std::cout << "\n";
  }

  delete[] referenceBit;
  report();
}

void Replace::eclock(void)
{
  int i, j, k, next;
  int pointer = 0;
  bool *referenceBit = new bool[frameNumber];
  bool *modifyBit = new bool[frameNumber];

  initSpace("Enhanced CLOCK");

  // 初始化引用位和修改位
  for (i = 0; i < frameNumber; i++)
  {
    referenceBit[i] = false;
    modifyBit[i] = false;
  }

  for (k = 0, j = 0; k < pageNumber; k++)
  {
    next = referencePage[k];

    // 检查页面是否已在内存中
    for (i = 0; i < frameNumber; i++)
    {
      if (next == pageFrames[i])
      {
        referenceBit[i] = true;
        // 模拟页面修改：写操作概率为30%
        if (rand() % 100 < 30) {
          modifyBit[i] = true;
          std::cout << "(写) "; // 标记写操作
        }
        break;
      }
    }

    if (i < frameNumber)
    {
      for (i = 0; i < frameNumber; i++)
        if (pageFrames[i] >= 0)
          std::cout << pageFrames[i] << " ";
      std::cout << "\n";
      continue;
    }

    faultNumber++;

    // Enhanced Clock算法的四个阶段
    int startPointer = pointer;
    bool found = false;
    int phase = 1;

    while (!found && phase <= 4)
    {
      int scanCount = 0;
      do {
        if (pageFrames[pointer] == -1) {
          found = true;
          break;
        }
        
        switch (phase) {
          case 1: // 寻找(0,0)
            if (!referenceBit[pointer] && !modifyBit[pointer]) {
              found = true;
            }
            break;
          case 2: // 寻找(0,1)
            if (!referenceBit[pointer] && modifyBit[pointer]) {
              found = true;
            }
            break;
          case 3: // 寻找(1,0)，清除引用位
            if (referenceBit[pointer] && !modifyBit[pointer]) {
              found = true;
            }
            referenceBit[pointer] = false;
            break;
          case 4: // 寻找(1,1)，清除引用位
            if (referenceBit[pointer] && modifyBit[pointer]) {
              found = true;
            }
            referenceBit[pointer] = false;
            break;
        }
        
        if (!found) {
          pointer = (pointer + 1) % frameNumber;
          scanCount++;
        }
      } while (!found && scanCount < frameNumber);
      
      if (!found) {
        phase++;
        pointer = startPointer; // 重新开始扫描
      }
    }

    if (!found) {
      pointer = startPointer; // 如果仍未找到，使用起始位置
    }

    eliminatePage[j] = pageFrames[pointer];
    pageFrames[pointer] = next;
    referenceBit[pointer] = true;
    modifyBit[pointer] = false; // 新加载的页面未被修改
    pointer = (pointer + 1) % frameNumber;

    for (i = 0; i < frameNumber; i++)
      if (pageFrames[i] >= 0)
        std::cout << pageFrames[i] << " ";

    if (eliminatePage[j] >= 0)
      std::cout << "->" << eliminatePage[j++] << "\n";
    else
      std::cout << "\n";
  }

  delete[] referenceBit;
  delete[] modifyBit;
  report();
}

void Replace::lfu(void)
{
  int i, j, k, next, minFreq, minIndex;
  int *frequency = new int[frameNumber];

  initSpace("LFU");

  // 初始化频率数组
  for (i = 0; i < frameNumber; i++)
    frequency[i] = 0;

  for (k = 0, j = 0; k < pageNumber; k++)
  {
    next = referencePage[k];

    // 检查页面是否已在内存中
    for (i = 0; i < frameNumber; i++)
    {
      if (next == pageFrames[i])
      {
        frequency[i]++; // 增加访问频率
        break;
      }
    }

    if (i < frameNumber)
    {
      for (i = 0; i < frameNumber; i++)
        if (pageFrames[i] >= 0)
          std::cout << pageFrames[i] << " ";
      std::cout << "\n";
      continue;
    }

    faultNumber++;

    // 查找空闲页框或频率最低的页框
    minIndex = 0;
    for (i = 0; i < frameNumber; i++)
    {
      if (pageFrames[i] == -1)
      {
        minIndex = i;
        break;
      }
      if (frequency[i] < frequency[minIndex])
        minIndex = i;
    }

    eliminatePage[j] = pageFrames[minIndex];
    pageFrames[minIndex] = next;
    frequency[minIndex] = 1; // 新页面频率为1

    for (i = 0; i < frameNumber; i++)
      if (pageFrames[i] >= 0)
        std::cout << pageFrames[i] << " ";

    if (eliminatePage[j] >= 0)
      std::cout << "->" << eliminatePage[j++] << "\n";
    else
      std::cout << "\n";
  }

  delete[] frequency;
  report();
}

void Replace::mfu(void)
{
  int i, j, k, next, maxFreq, maxIndex;
  int *frequency = new int[frameNumber];

  initSpace("MFU");

  // 初始化频率数组
  for (i = 0; i < frameNumber; i++)
    frequency[i] = 0;

  for (k = 0, j = 0; k < pageNumber; k++)
  {
    next = referencePage[k];

    // 检查页面是否已在内存中
    for (i = 0; i < frameNumber; i++)
    {
      if (next == pageFrames[i])
      {
        frequency[i]++; // 增加访问频率
        break;
      }
    }

    if (i < frameNumber)
    {
      for (i = 0; i < frameNumber; i++)
        if (pageFrames[i] >= 0)
          std::cout << pageFrames[i] << " ";
      std::cout << "\n";
      continue;
    }

    faultNumber++;

    // 查找空闲页框或频率最高的页框
    maxIndex = 0;
    for (i = 0; i < frameNumber; i++)
    {
      if (pageFrames[i] == -1)
      {
        maxIndex = i;
        break;
      }
      if (frequency[i] > frequency[maxIndex])
        maxIndex = i;
    }

    eliminatePage[j] = pageFrames[maxIndex];
    pageFrames[maxIndex] = next;
    frequency[maxIndex] = 1; // 新页面频率为1

    for (i = 0; i < frameNumber; i++)
      if (pageFrames[i] >= 0)
        std::cout << pageFrames[i] << " ";

    if (eliminatePage[j] >= 0)
      std::cout << "->" << eliminatePage[j++] << "\n";
    else
      std::cout << "\n";
  }

  delete[] frequency;
  report();
}

void Replace::generateRandomSequence(int pageRange)
{
  srand(time(NULL));
  std::cout << "随机生成的页面引用串：";
  for (int i = 0; i < pageNumber; i++) {
    referencePage[i] = rand() % pageRange;
    std::cout << referencePage[i] << " ";
  }
  std::cout << "\n";
}

void Replace::resetData(void)
{
  // 重置数据以便重复测试，但保持引用串不变
  for (int i = 0; i < pageNumber; i++)
    eliminatePage[i] = -1;
  for (int i = 0; i < frameNumber; i++)
    pageFrames[i] = -1;
  faultNumber = 0;
}

void Replace::compareAlgorithms(void)
{
  struct AlgorithmResult {
    const char* name;
    int faults;
    double rate;
  };
  
  AlgorithmResult results[6];
  
  // 测试LRU
  resetData();
  lru();
  results[0] = {"LRU", faultNumber, (double)faultNumber/pageNumber*100};
  
  // 测试FIFO
  resetData();
  fifo();
  results[1] = {"FIFO", faultNumber, (double)faultNumber/pageNumber*100};
  
  // 测试CLOCK
  resetData();
  clock();
  results[2] = {"CLOCK", faultNumber, (double)faultNumber/pageNumber*100};
  
  // 测试Enhanced CLOCK
  resetData();
  eclock();
  results[3] = {"Enhanced CLOCK", faultNumber, (double)faultNumber/pageNumber*100};
  
  // 测试LFU
  resetData();
  lfu();
  results[4] = {"LFU", faultNumber, (double)faultNumber/pageNumber*100};
  
  // 测试MFU
  resetData();
  mfu();
  results[5] = {"MFU", faultNumber, (double)faultNumber/pageNumber*100};
  
  // 输出比较结果
  std::cout << "\n========== 性能比较结果 ==========\n";
  std::cout << std::left << std::setw(15) << "算法名称" 
            << std::setw(10) << "缺页次数" 
            << std::setw(10) << "缺页率(%)" << "\n";
  std::cout << "----------------------------------------\n";
  
  for (int i = 0; i < 6; i++) {
    std::cout << std::left << std::setw(15) << results[i].name
              << std::setw(10) << results[i].faults
              << std::fixed << std::setprecision(2) << results[i].rate << "%\n";
  }
}

void Replace::generateBelady(void)
{
  // Belady测试序列：7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1
  int belady[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
  int beladySize = sizeof(belady) / sizeof(belady[0]);
  
  if (pageNumber > beladySize) {
    pageNumber = beladySize;
    std::cout << "使用Belady序列，共" << pageNumber << "个页面\n";
  }
  
  std::cout << "使用Belady测试序列：";
  for (int i = 0; i < pageNumber; i++) {
    referencePage[i] = belady[i];
    std::cout << referencePage[i] << " ";
  }
  std::cout << "\n";
}

int main(int argc, char *argv[])
{
  Replace *vmpr = new Replace();
  
  // 运行所有算法并进行性能比较
  vmpr->compareAlgorithms();
  
  delete vmpr;
  return 0;
}