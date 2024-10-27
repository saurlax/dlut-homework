/**
 * 随机产生20个不大于10的自然数，将其存储在一个二维数组中，
 * 且偶数与奇数分别存储在不同列上。输出该数组并统计偶数与奇数的个数。
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
  int arr[2][20];
  int even = 0, odd = 0;
  srand(time(0));

  for (int i = 0; i < 20; i++)
  {
    int num = rand() % 10;
    if (num % 2)
    {
      arr[0][odd++] = num;
    }
    else
    {
      arr[1][even++] = num;
    }
  }

  puts("The array is:");
  for (int i = 0; i < odd; i++)
  {
    printf("%d ", arr[0][i]);
  }
  puts("");
  for (int i = 0; i < even; i++)
  {
    printf("%d ", arr[1][i]);
  }
  printf("\nOdd: %d, Even: %d\n", odd, even);
  return 0;
}