/**
 * 完成如下函数：
 * void invert(int(*a)[N]);  //将N*N的矩阵以对角线为轴翻转（两条对角线）
 */
#include <stdio.h>
#define N 3

void invert(int (*a)[N])
{
  int i, j, temp;
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < i; j++)
    {
      temp = a[i][j];
      a[i][j] = a[j][i];
      a[j][i] = temp;
    }
  }
}

int main()
{
  int mat[N][N] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  invert(mat);
  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < N; ++j)
    {
      printf("%d ", mat[i][j]);
    }
    printf("\n");
  }
  return 0;
}