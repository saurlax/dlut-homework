/**
 * 输出一个一维数组，然后该数组前后翻转，并输出。
 */
#include <stdio.h>

int array[10000];
int len = 0;

int main(void)
{
  puts("Please input an array:");
  do
  {
    scanf("%d", &array[len++]);
  } while (getchar() != '\n');

  for (int i = 0; i < len / 2; i++)
  {
    int temp = array[i];
    array[i] = array[len - i - 1];
    array[len - i - 1] = temp;
  }

  puts("The inverted array is:");
  for (int i = 0; i < len; i++)
  {
    printf("%d ", array[i]);
  }
  return 0;
}