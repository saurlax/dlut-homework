/**
 * 声明一个结构体Point（包括int x、y坐标）。完成函数用于输出结构体变量的坐标。
 * void outputPoint(struct Point p); // 输出点的坐标,输出格式[x, y]
 * void outputPoints(struct Point p[],int n); // 输出一组点的坐标，需调用outputPoint函数
 * int main() {
 *   struct Point p[3]={0,0,1,1,2,2};
 *   outputPoints(p,3);
 * }
 * 输出：
 * [0, 0]
 * [1, 1]
 * [2, 2]
 * 练习结构体的声明、变量定义、结构体数组、结构体作为函数参数
 */
#include <stdio.h>

struct Point
{
  int x;
  int y;
};

void outputPoint(struct Point p)
{
  printf("[%d, %d]\n", p.x, p.y);
}

void outputPoints(struct Point p[], int n)
{
  for (int i = 0; i < n; i++)
    outputPoint(p[i]);
}

int main()
{
  struct Point p[3] = {0, 0, 1, 1, 2, 2};
  outputPoints(p, 3);
  return 0;
}