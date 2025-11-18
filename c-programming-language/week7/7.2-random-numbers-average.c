/**
 * 随机产生10个100以内的自然数（保存到数组），统计大于等于平均值的元素个数。
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int array[10];
double average = 0;
int count = 0;

int main()
{
  srand(time(0));
  for (int i = 0; i < 10; i++)
  {
    array[i] = rand() % 100;
    average += array[i] / 10.0;
  }
  puts("The array is:");
  for (int i = 0; i < 10; i++)
  {
    printf("%d ", array[i]);
  }
  puts("");

  for (int i = 0; i < 10; i++)
  {
    if (array[i] >= average)
    {
      count++;
    }
  }
  printf("There are %d numbers greater than or equal to the average %.2lf.\n", count, average);
  return 0;
}