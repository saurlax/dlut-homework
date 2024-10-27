/**
 * 有如下结构体Circle，描述平面圆。编写函数计算圆的面积，并根据面积对若干个圆进行排序。
 * 并在主函数中排序后输出，输出时可使用上一题中的outputPoint函数。
 * #include <stdio.h>
 * #define N 3
 * #define PI 3.14
 * struct Point //平面点
 * {
 *   int x，y; //坐标
 * };
 * struct Circle // 圆形
 * {
 *   struct Point pt; //圆心
 *   double r; //半径
 *   double area; // 面积
 * };
 * double getArea(struct Circle* pC); // 计算某圆形的面积
 * void sortCir(struct Circle c[], int n); // 根据面积对包含n个圆形数组n降序排序
 * int main()
 * {
 *   struct Circle c[N]={{0,0,1},{1,1,1.1},{1,2,2.2}};
 *   int i;
 *   for(i=0;i<N;i++)
 *   c[i].area = getArea(c+i);
 *   sortCir(c,N);
 *   for(i=0;i<N;i++)
 *   {
 *     outputPoint(c[i].pt);
 *     printf(", r=%f, area=%f\n",c[i].r, c[i].area);
 *   }
 *   return 0;
 * }
 * 输出：
 * [1,2], r=2.200000, area=15.197600
 * [1,1], r=1.100000, area=3.799400
 * [0,0], r=1.000000, area=3.140000
 */
#include <stdio.h>
#define N 3
#define PI 3.14

struct Point
{
  int x, y;
};

struct Circle
{
  struct Point pt;
  double r;
  double area;
};

void outputPoint(struct Point pt)
{
  printf("[%d,%d]", pt.x, pt.y);
}

double getArea(struct Circle *pC)
{
  return PI * pC->r * pC->r;
}

void sortCir(struct Circle c[], int n)
{
  for (int i = 0; i < n - 1; i++)
  {
    int max = i;
    for (int j = i + 1; j < n; j++)
    {
      if (c[j].area > c[max].area)
      {
        max = j;
      }
    }
    if (max != i)
    {
      struct Circle temp = c[i];
      c[i] = c[max];
      c[max] = temp;
    }
  }
}

int main()
{
  struct Circle c[N] = {{0, 0, 1}, {1, 1, 1.1}, {1, 2, 2.2}};
  for (int i = 0; i < N; i++)
    c[i].area = getArea(c + i);
  sortCir(c, N);
  for (int i = 0; i < N; i++)
  {
    outputPoint(c[i].pt);
    printf(", r=%f, area=%f\n", c[i].r, c[i].area);
  }
  return 0;
}