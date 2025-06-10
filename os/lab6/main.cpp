#include "disk.h"

int *generateRandomRequests(int requestNumber, int maxCylinder)
{
  srand(time(nullptr));
  int *requests = new int[requestNumber];

  std::cout << "随机请求序列: ";

  for (int i = 0; i < requestNumber; i++)
  {
    requests[i] = rand() % (maxCylinder + 1);
    std::cout << requests[i];
    if (i < requestNumber - 1)
      std::cout << ", ";
  }
  std::cout << std::endl;

  return requests;
}

int main()
{
  int choice;
  std::cout << "磁盘移臂调度算法" << std::endl;
  std::cout << "1. 手动输入请求序列" << std::endl;
  std::cout << "2. 随机生成请求序列" << std::endl;
  std::cout << "选择输入方式: ";
  std::cin >> choice;

  int currentCylinder, seekDirection, requestNumber;
  int *requests;

  std::cout << "请输入当前磁头位置: ";
  std::cin >> currentCylinder;
  std::cout << "请输入当前磁头方向 (0-向左/1-向右): ";
  std::cin >> seekDirection;
  std::cout << "请输入磁盘请求数量: ";
  std::cin >> requestNumber;

  if (choice == 1)
  {
    // 手动输入请求序列
    requests = new int[requestNumber];
    std::cout << "请输入磁盘请求序列: ";
    for (int i = 0; i < requestNumber; i++)
    {
      std::cin >> requests[i];
    }
  }
  else
  {
    // 随机生成请求序列
    int maxCylinder;
    std::cout << "请输入最大柱面号: ";
    std::cin >> maxCylinder;
    requests = generateRandomRequests(requestNumber, maxCylinder);
  }

  DiskArm disk(currentCylinder, seekDirection, requestNumber, requests);

  // 执行所有算法进行对比
  disk.fcfs();
  disk.sstf();
  disk.scan();
  disk.cscan();
  disk.look();

  // 清理内存
  delete[] requests;

  return 0;
}
