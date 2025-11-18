# 实验六：磁盘移臂调度算法实验

https://github.com/saurlax/dlut-homework/tree/main/os/lab6

20232241392-文浩然-软 2305

## 算法实现

### FCFS (先来先服务)

**算法流程：**

1. 按照请求到达的顺序依次处理
2. 磁头直接从当前位置移动到下一个请求位置
3. 记录每次移动的距离和方向变化

**核心代码：**

```cpp
void DiskArm::fcfs()
{
  int current = currentCylinder;
  int direction = seekDirection;

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
}
```

**特点分析：**

- 优点：简单公平，无饥饿现象
- 缺点：寻道距离可能很大，效率低

### SSTF (最短寻道时间优先)

**算法流程：**

1. 在所有未处理的请求中找到距离当前位置最近的
2. 移动到该位置并标记为已处理
3. 重复直到所有请求处理完毕

**核心代码：**

```cpp
void DiskArm::sstf()
{
  bool *visited = new bool[requestNumber];

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

    // 处理找到的请求
    visited[nextIndex] = true;
    // 检查方向变化并移动
  }
}
```

**特点分析：**

- 优点：寻道距离相对较短，效率较高
- 缺点：可能产生饥饿现象，远端请求长时间得不到服务

### SCAN (电梯算法)

**算法流程：**

1. 磁头朝一个方向移动，处理路径上的所有请求
2. 到达边界后调头，处理另一个方向的请求
3. 类似电梯的工作方式

**核心代码：**

```cpp
void DiskArm::scan()
{
  // 分离左右请求
  for (int i = 0; i < requestNumber; i++)
  {
    if (cylinder[i] < current)
      leftRequests[leftCount++] = cylinder[i];
    else
      rightRequests[rightCount++] = cylinder[i];
  }

  // 排序：左边从大到小，右边从小到大
  std::sort(leftRequests, leftRequests + leftCount, std::greater<int>());
  std::sort(rightRequests, rightRequests + rightCount);

  if (direction == 1) // 向右移动
  {
    // 先处理右边请求，再调头处理左边
    for (int i = 0; i < rightCount; i++) { /* 处理右边 */ }
    if (leftCount > 0)
    {
      seekChange++; // 调头
      for (int i = 0; i < leftCount; i++) { /* 处理左边 */ }
    }
  }
}
```

**特点分析：**

- 优点：避免饥饿，寻道距离适中
- 缺点：边界处的请求响应时间较长

### C-SCAN (循环扫描)

**算法流程：**

1. 磁头朝一个方向移动处理请求
2. 到达边界后直接跳转到另一端继续同方向扫描
3. 形成循环，提供更均匀的服务

**核心代码：**

```cpp
void DiskArm::cscan()
{
  if (direction == 1) // 向右移动
  {
    // 处理右边请求
    for (int i = 0; i < rightCount; i++) { /* 处理 */ }

    if (leftCount > 0)
    {
      seekChange++; // 跳转
      std::cout << " [跳转] [调头]";
      // 从最小的左边请求开始处理
      for (int i = 0; i < leftCount; i++) { /* 处理 */ }
    }
  }
}
```

**特点分析：**

- 优点：提供更均匀的等待时间
- 缺点：可能增加不必要的移动距离

### LOOK (电梯优化算法)

**算法流程：**

1. 类似 SCAN，但不需要到达磁盘边界
2. 当某个方向没有更多请求时就调头
3. 减少不必要的边界移动

**核心代码：**

```cpp
void DiskArm::look()
{
  if (direction == 1) // 向右移动
  {
    // 处理右边的请求
    for (int i = 0; i < rightCount; i++) { /* 处理 */ }

    // 如果还有左边的请求，调头处理
    if (leftCount > 0)
    {
      seekChange++;
      std::cout << " [调头]";
      for (int i = 0; i < leftCount; i++) { /* 处理 */ }
    }
  }
}
```

**特点分析：**

- 优点：结合 SCAN 优点，减少不必要移动
- 缺点：实现相对复杂

## 数据结构设计

### DiskArm 类设计

```cpp
class DiskArm
{
private:
  int currentCylinder;    // 当前磁头位置
  int seekDirection;      // 移动方向 (0:左, 1:右)
  int requestNumber;      // 请求数量
  int *request;          // 原始请求序列
  int *cylinder;         // 工作用请求序列
  int maxCylinder;       // 最大柱面号
  int seekNumber;        // 总寻道距离
  int seekChange;        // 方向改变次数

public:
  DiskArm(int currentCylinder, int seekDirection, int requestNumber, int *requests);
  ~DiskArm();

  void fcfs();    // FCFS算法
  void sstf();    // SSTF算法
  void scan();    // SCAN算法
  void cscan();   // C-SCAN算法
  void look();    // LOOK算法

private:
  void initSpace(const char *methodName);  // 初始化
  void report();                           // 结果报告
};
```

### 关键成员变量说明

- `seekNumber`: 累计寻道距离，用于计算总距离和平均距离
- `seekChange`: 方向改变次数，用于评估算法的移动模式
- `cylinder[]`: 工作数组，避免修改原始请求数据

## 输出格式设计

每种算法的输出包含：

1. 算法名称和分隔线
2. 初始方向显示
3. 详细的访问序列，包括调头标记
4. 统计信息：总寻道距离、平均寻道距离、方向改变次数

**输出示例：**

```
========================================
FCFS (先来先服务)
初始方向: 向右
访问序列: 100 -> 200 -> [调头] 50 -> 150
总寻道距离: 350
平均寻道距离: 116.67
方向改变次数: 1
```

## 性能分析

### 评估指标

1. **总寻道距离**: 反映算法的总体效率
2. **平均寻道距离**: 标准化的效率指标
3. **方向改变次数**: 反映算法的稳定性

### 算法比较

| 算法   | 时间复杂度 | 空间复杂度 | 适用场景               |
| ------ | ---------- | ---------- | ---------------------- |
| FCFS   | O(n)       | O(1)       | 请求较少或公平性要求高 |
| SSTF   | O(n²)      | O(n)       | 追求最短寻道距离       |
| SCAN   | O(n log n) | O(n)       | 一般情况，避免饥饿     |
| C-SCAN | O(n log n) | O(n)       | 要求均匀响应时间       |
| LOOK   | O(n log n) | O(n)       | 优化的一般选择         |

## 实验结论

1. **FCFS 算法**简单但效率低，适合轻负载场景
2. **SSTF 算法**效率高但可能造成饥饿，适合重负载场景
3. **SCAN 算法**平衡了效率和公平性，是经典的实用算法
4. **C-SCAN 算法**提供更均匀的服务，适合实时系统
5. **LOOK 算法**在 SCAN 基础上优化，是现代系统的常用选择

在实际应用中，应根据具体的 I/O 模式、负载特征和性能要求选择合适的调度算法。
