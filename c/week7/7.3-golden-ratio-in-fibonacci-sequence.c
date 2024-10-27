/**
 * (1) 利用数组保存Fibonacci数列的前20项；
 * (2) 验证F(n-1)/F(n)是否接近黄金分割数（0.618）。
 */
#include <stdio.h>

int fibonacci[20];

int main()
{
  fibonacci[0] = 1;
  fibonacci[1] = 1;
  for (int i = 2; i < 20; i++)
  {
    fibonacci[i] = fibonacci[i - 1] + fibonacci[i - 2];
  }

  double average = 0;
  double max = 1;
  double min = 1;
  for (int i = 1; i < 20; i++)
  {
    double ratio = (double)fibonacci[i - 1] / fibonacci[i];
    average += ratio / 20;
    if (ratio > max)
    {
      max = ratio;
    }
    if (ratio < min)
    {
      min = ratio;
    }
  }
  printf("average ratio: %lf max ratio: %lf min ratio: %lf\n", average, max, min);
  return 0;
}