/**
 * 编写函数将数组中的元素按照逆序存储。请用“数组下标法”与“指针法”分别实现。函数原型如下：
 * void invert( int* a, int n);
 */
#include <stdio.h>

void invertByIndex(int *a, int n)
{
  int temp;
  for (int i = 0; i < n / 2; i++)
  {
    temp = a[i];
    a[i] = a[n - i - 1];
    a[n - i - 1] = temp;
  }
}

void invertByPointer(int *a, int n)
{
  int temp;
  for (int *p = a, *q = a + n - 1; p < q; p++, q--)
  {
    temp = *p;
    *p = *q;
    *q = temp;
  }
}

int arr[100];
int len = 0;

int main()
{
  int method;
  puts("Please input the array:");
  do
  {
    scanf("%d", &arr[len++]);
  } while (getchar() != '\n');
  puts("Which method do you want to use? (1. index, 2. pointer)");
  scanf("%d", &method);
  switch (method)
  {
  case 1:
    invertByIndex(arr, len);
    break;
  case 2:
    invertByPointer(arr, len);
    break;
  default:
    puts("Invalid method.");
    return 0;
  }
  puts("The result is:");
  for (int i = 0; i < len; i++)
  {
    printf("%d ", arr[i]);
  }
  return 0;
}