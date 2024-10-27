/**
 * 在主函数中定义一个float数组，完成下列函数
 * （1）编写函数，可获得数组中最大值的地址，在主函数中利用该函数获得该地址，并输出该数组最大值（如果该值出现多次，则输出下标较小的）。函数原型如下：
 * void getMax(float * arr, int n, float *pMax);
 * 其中arr为数组首地址，n为数组长度，pMax为指向最大值的指针。
 * （2）编写函数获得数组中最小值的地址，在主函数中利用该函数获得该地址，并输出该最小值（如果该值出现多次，则输出下标较小的）。函数原型如下：
 * float * getMin(float * arr, int n);
 * 其中arr为数组首地址，n为数组长度，该函数返回为最小值地址。
 */
#include <stdio.h>

float *max(float arr[], int n)
{
  float *ptr = arr;
  for (int i = 0; i < n; i++)
    if (arr[i] > *ptr)
      ptr = &arr[i];
  return ptr;
}

float *min(float arr[], int n)
{
  float *ptr = arr;
  for (int i = 0; i < n; i++)
    if (arr[i] < *ptr)
      ptr = &arr[i];
  return ptr;
}

float arr[100];
int len = 0;

int main()
{
  puts("Please input the array:");
  do
  {
    scanf("%f", &arr[len++]);
  } while (getchar() != '\n');
  printf("The max value is %f\n", *max(arr, len));
  printf("The min value is %f\n", *min(arr, len));
  return 0;
}