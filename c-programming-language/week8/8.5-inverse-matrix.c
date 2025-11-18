/**
 * 编写函数 void inverse(int arr[][N])，
 * 实现N*N的矩阵转置操作, 其中N为常量。（二维数组作为函数参数）
 */
#include <stdio.h>
#define N 3

void inverse(int arr[N][N])
{
  int temp;
  for (int i = 0; i < N; i++)
  {
    for (int j = i; j < N; j++)
    {
      temp = arr[i][j];
      arr[i][j] = arr[j][i];
      arr[j][i] = temp;
    }
  }
}

int main()
{
  int arr[N][N];
  puts("Please input a 3*3 matrix:");
  for (int i = 0; i < N * N; i++)
  {
    scanf("%d", &arr[i / N][i % N]);
  }
  inverse(arr);
  puts("The inverse matrix is:");
  for (int i = 0; i < N * N; i++)
  {
    printf("%d ", arr[i / N][i % N]);
    if (i % N == N - 1)
      putchar('\n');
  }
  return 0;
}