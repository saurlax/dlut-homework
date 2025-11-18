#include "disk.h"
#include <climits>

DiskArm::DiskArm(int currentCylinder, int seekDirection, int requestNumber, int *requests)
{
  this->currentCylinder = currentCylinder;
  this->seekDirection = seekDirection;
  this->requestNumber = requestNumber;

  // 分配内存
  request = new int[requestNumber];
  cylinder = new int[requestNumber];

  // 复制请求数据并计算最大柱面号
  maxCylinder = currentCylinder;
  for (int i = 0; i < requestNumber; i++)
  {
    request[i] = requests[i];
    if (requests[i] > maxCylinder)
      maxCylinder = requests[i];
  }
}

DiskArm::~DiskArm()
{
  delete[] request;
  delete[] cylinder;
}

// 初始化道号，寻道记录
void DiskArm::initSpace(const char *methodName)
{
  std::cout << std::endl
            << "========================================" << std::endl;
  std::cout << methodName << std::endl;
  seekNumber = 0;
  seekChange = 0;
  for (int i = 0; i < requestNumber; i++)
    cylinder[i] = request[i];
}

// 统计报告算法执⾏情况
void DiskArm::report()
{
  std::cout << "总寻道距离: " << seekNumber << std::endl;
  std::cout << "平均寻道距离: " << (double)seekNumber / requestNumber << std::endl;
  std::cout << "方向改变次数: " << seekChange << std::endl;
}

void DiskArm::fcfs()
{
  int current = currentCylinder;
  int direction = seekDirection;
  initSpace("FCFS (先来先服务)");
  std::cout << "初始方向: " << (direction ? "向右" : "向左") << std::endl;
  std::cout << "访问序列: " << current;

  for (int i = 0; i < requestNumber; i++)
  {
    // 检查是否需要改变方向
    int newDirection = (cylinder[i] >= current) ? 1 : 0;
    if (newDirection != direction)
    {
      seekChange++;
      direction = newDirection;
      std::cout << " -> [调头] " << cylinder[i];
    }
    else
    {
      std::cout << " -> " << cylinder[i];
    }

    seekNumber += abs(current - cylinder[i]);
    current = cylinder[i];
  }

  std::cout << std::endl;
  report();
}

void DiskArm::sstf()
{
  initSpace("SSTF (最短寻道时间优先)");
  int current = currentCylinder;
  int direction = seekDirection;
  bool *visited = new bool[requestNumber];
  for (int i = 0; i < requestNumber; i++)
    visited[i] = false;

  std::cout << "初始方向: " << (direction ? "向右" : "向左") << std::endl;
  std::cout << "访问序列: " << current;

  for (int count = 0; count < requestNumber; count++)
  {
    int minDistance = INT_MAX;
    int nextIndex = -1;

    // 找到距离当前位置最近的未访问请求
    for (int i = 0; i < requestNumber; i++)
    {
      if (!visited[i])
      {
        int distance = abs(current - cylinder[i]);
        if (distance < minDistance)
        {
          minDistance = distance;
          nextIndex = i;
        }
      }
    }

    if (nextIndex != -1)
    {
      visited[nextIndex] = true;

      // 检查是否需要改变方向
      int newDirection = (cylinder[nextIndex] >= current) ? 1 : 0;
      if (newDirection != direction)
      {
        seekChange++;
        direction = newDirection;
        std::cout << " -> [调头] " << cylinder[nextIndex];
      }
      else
      {
        std::cout << " -> " << cylinder[nextIndex];
      }

      seekNumber += abs(current - cylinder[nextIndex]);
      current = cylinder[nextIndex];
    }
  }

  std::cout << std::endl;
  delete[] visited;
  report();
}

void DiskArm::scan()
{
  initSpace("SCAN (电梯算法)");
  int current = currentCylinder;
  int direction = seekDirection;

  // 创建两个数组分别存储当前方向和反方向的请求
  int *leftRequests = new int[requestNumber];
  int *rightRequests = new int[requestNumber];
  int leftCount = 0, rightCount = 0;

  // 分离请求
  for (int i = 0; i < requestNumber; i++)
  {
    if (cylinder[i] < current)
      leftRequests[leftCount++] = cylinder[i];
    else if (cylinder[i] > current)
      rightRequests[rightCount++] = cylinder[i];
    else
      rightRequests[rightCount++] = cylinder[i]; // 等于当前位置的放右边
  }

  // 排序
  std::sort(leftRequests, leftRequests + leftCount, std::greater<int>());
  std::sort(rightRequests, rightRequests + rightCount);

  std::cout << "初始方向: " << (direction ? "向右" : "向左") << std::endl;
  std::cout << "访问序列: " << current;

  if (direction == 1) // 向右移动
  {
    // 先处理右边的请求
    for (int i = 0; i < rightCount; i++)
    {
      std::cout << " -> " << rightRequests[i];
      seekNumber += abs(current - rightRequests[i]);
      current = rightRequests[i];
    }

    if (leftCount > 0)
    {
      seekChange++; // 调头
      std::cout << " [调头]";
      // 再处理左边的请求
      for (int i = 0; i < leftCount; i++)
      {
        std::cout << " -> " << leftRequests[i];
        seekNumber += abs(current - leftRequests[i]);
        current = leftRequests[i];
      }
    }
  }
  else // 向左移动
  {
    // 先处理左边的请求
    for (int i = 0; i < leftCount; i++)
    {
      std::cout << " -> " << leftRequests[i];
      seekNumber += abs(current - leftRequests[i]);
      current = leftRequests[i];
    }

    if (rightCount > 0)
    {
      seekChange++; // 调头
      std::cout << " [调头]";
      // 再处理右边的请求
      for (int i = 0; i < rightCount; i++)
      {
        std::cout << " -> " << rightRequests[i];
        seekNumber += abs(current - rightRequests[i]);
        current = rightRequests[i];
      }
    }
  }

  std::cout << std::endl;
  delete[] leftRequests;
  delete[] rightRequests;
  report();
}

void DiskArm::cscan()
{
  initSpace("C-SCAN (循环扫描)");
  int current = currentCylinder;
  int direction = seekDirection;

  int *leftRequests = new int[requestNumber];
  int *rightRequests = new int[requestNumber];
  int leftCount = 0, rightCount = 0;

  // 分离请求
  for (int i = 0; i < requestNumber; i++)
  {
    if (cylinder[i] < current)
      leftRequests[leftCount++] = cylinder[i];
    else if (cylinder[i] > current)
      rightRequests[rightCount++] = cylinder[i];
    else
      rightRequests[rightCount++] = cylinder[i];
  }

  // 排序
  std::sort(leftRequests, leftRequests + leftCount);
  std::sort(rightRequests, rightRequests + rightCount);

  std::cout << "初始方向: " << (direction ? "向右" : "向左") << std::endl;
  std::cout << "访问序列: " << current;

  if (direction == 1) // 向右移动
  {
    // 先处理右边的请求
    for (int i = 0; i < rightCount; i++)
    {
      std::cout << " -> " << rightRequests[i];
      seekNumber += abs(current - rightRequests[i]);
      current = rightRequests[i];
    }

    if (leftCount > 0)
    {
      seekChange++; // 跳转到最小位置
      std::cout << " [跳转] [调头]";
      // 从最小的左边请求开始处理
      for (int i = 0; i < leftCount; i++)
      {
        std::cout << " -> " << leftRequests[i];
        seekNumber += abs(current - leftRequests[i]);
        current = leftRequests[i];
      }
    }
  }
  else // 向左移动
  {
    // 先处理左边的请求（从大到小）
    for (int i = leftCount - 1; i >= 0; i--)
    {
      std::cout << " -> " << leftRequests[i];
      seekNumber += abs(current - leftRequests[i]);
      current = leftRequests[i];
    }

    if (rightCount > 0)
    {
      seekChange++; // 跳转到最大位置
      std::cout << " [跳转] [调头]";
      // 从最大的右边请求开始处理
      for (int i = rightCount - 1; i >= 0; i--)
      {
        std::cout << " -> " << rightRequests[i];
        seekNumber += abs(current - rightRequests[i]);
        current = rightRequests[i];
      }
    }
  }

  std::cout << std::endl;
  delete[] leftRequests;
  delete[] rightRequests;
  report();
}

void DiskArm::look()
{
  initSpace("LOOK (电梯优化算法)");
  int current = currentCylinder;
  int direction = seekDirection;

  int *leftRequests = new int[requestNumber];
  int *rightRequests = new int[requestNumber];
  int leftCount = 0, rightCount = 0;

  // 分离请求
  for (int i = 0; i < requestNumber; i++)
  {
    if (cylinder[i] < current)
      leftRequests[leftCount++] = cylinder[i];
    else if (cylinder[i] > current)
      rightRequests[rightCount++] = cylinder[i];
    else
      rightRequests[rightCount++] = cylinder[i];
  }

  // 排序
  std::sort(leftRequests, leftRequests + leftCount, std::greater<int>());
  std::sort(rightRequests, rightRequests + rightCount);

  std::cout << "初始方向: " << (direction ? "向右" : "向左") << std::endl;
  std::cout << "访问序列: " << current;

  if (direction == 1) // 向右移动
  {
    // 处理右边的请求
    for (int i = 0; i < rightCount; i++)
    {
      std::cout << " -> " << rightRequests[i];
      seekNumber += abs(current - rightRequests[i]);
      current = rightRequests[i];
    }

    // 如果还有左边的请求，调头处理
    if (leftCount > 0)
    {
      seekChange++;
      std::cout << " [调头]";
      for (int i = 0; i < leftCount; i++)
      {
        std::cout << " -> " << leftRequests[i];
        seekNumber += abs(current - leftRequests[i]);
        current = leftRequests[i];
      }
    }
  }
  else // 向左移动
  {
    // 处理左边的请求
    for (int i = 0; i < leftCount; i++)
    {
      std::cout << " -> " << leftRequests[i];
      seekNumber += abs(current - leftRequests[i]);
      current = leftRequests[i];
    }

    // 如果还有右边的请求，调头处理
    if (rightCount > 0)
    {
      seekChange++;
      std::cout << " [调头]";
      for (int i = 0; i < rightCount; i++)
      {
        std::cout << " -> " << rightRequests[i];
        seekNumber += abs(current - rightRequests[i]);
        current = rightRequests[i];
      }
    }
  }

  std::cout << std::endl;
  delete[] leftRequests;
  delete[] rightRequests;
  report();
}