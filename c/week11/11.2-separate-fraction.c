/**
 * 完成sep函数
 * int sep(double x, int *intptr)
 * 将提取浮点数 x 的整数部分和小数部分，整数部分被存入参数 intptr 指向单元中，小数部分被放入返回值中。
 * x：要被计算的浮点数；intptr：一个指向整数部分的指针。
 * 返回值：以整数形式返回 x 的小数部分（不确定小数位有几位，但不需要考虑超出整数长度限制的问题）。
 * int main() {
 *   double x = 12.345;   // 注意不确定小数部分有几位
 *   int i = 0;           // 整数部分
 *   int f = sep(x, &i);  // 求12.345的小数部分
 *   printf("intpart: %d\nfractpart: %d\n", i, f);
 *   return 0;
 * }
 * 运行结果：
 * intpart: 12
 * fractpart: 345
 * 思路：0.345乘10得到3.45，3.45减掉整数部分得到0.45，0.45大于1e-6，所以继续；
 * 3.45乘10得到34.5，减掉整数部分得到0.5，0. 5大于1e-6，所以继续；
 * 34.5乘10得到345.0，减掉整数部分得到0.0，0.0不大于1e-6，所以结束。
 * 最终345即为该数的小数部分
 */
#include <stdio.h>

int sep(double x, int *intptr)
{
  *intptr = (int)x;
  int res = 0;
  while (x - (int)x > 1e-6)
  {
    x -= (int)x;
    x *= 10;
    res = res * 10 + (int)x % 10;
  }
  return res;
}

int main()
{
  double x;
  printf("Please input a float number: ");
  scanf("%lf", &x);
  int i = 0;
  int f = sep(x, &i);
  printf("intpart: %d\nfractpart: %d\n", i, f);
  return 0;
}