/**
 * （1）编写swap函数，交换两个整型变量值
 * （2）利用swap函数，完成数组排序函数（冒泡升序）。函数原型如下：void sort(int arr[], int n);
 */
#include <stdio.h>

void swap(int *a, int *b)
{
  int temp = *a;
  *a = *b;
  *b = temp;
}

void bubble(int arr[], int n)
{
  for (int i = 0; i < n - 1; i++)
  {
    for (int j = 0; j < n - 1 - i; j++)
    {
      if (arr[j] > arr[j + 1])
      {
        swap(&arr[j], &arr[j + 1]);
      }
    }
  }
}

int arr[100];
int len = 0;

int main()
{
  puts("Please input the array:");
  do
  {
    scanf("%d", &arr[len++]);
  } while (getchar() != '\n');
  bubble(arr, len);
  puts("The sorted array is:");
  for (int i = 0; i < len; i++)
  {
    printf("%d ", arr[i]);
  }
  return 0;
}