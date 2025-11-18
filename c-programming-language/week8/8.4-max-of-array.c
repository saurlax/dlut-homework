/**
 * 编写函数 int max(int arr[], int n)，返回数组中最大元素的位置。
 * 在主函数中输出一维数组中最大元素的下标和值。（数组作为函数参数）
 * 思考: 能否调用该函数求出二维数组最大元素的位置？
 */
#include <stdio.h>

int arr[10000];
int len = 0;

int max(int arr[], int n)
{
  int max = 0;
  for (int i = 1; i < n; i++)
  {
    if (arr[i] > arr[max])
      max = i;
  }
  return max;
}

int main()
{
  // cannot convert 'int (*)[]' to 'int'
  puts("Please input the array:");
  do
  {
    scanf("%d", &arr[len++]);
  } while (getchar() != '\n');

  int i = max(arr, len);
  printf("max = arr[%d] = %d\n", i, arr[i]);
  return 0;
}