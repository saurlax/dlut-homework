/**
 * 编写函数用于找出二维数组中的所有鞍点，既该位置的元素在该行上最大，但是在该列上最小。
 * 也有可能没有鞍点。（二维数组作为函数参数）
 */
#include <stdio.h>
#define N 100
int arr[N][N];

void saddlePoint(int arr[N][N], int row, int col)
{
  int max[N], min[N];
  for (int i = 0; i < row; i++)
  {
    max[i] = 0;
    int temp = arr[i][0];
    for (int j = 1; j < col; j++)
    {
      if (arr[i][j] > max[i])
      {
        max[i] = j;
        temp = arr[i][j];
      }
    }
  }

  for (int j = 0; j < col; j++)
  {
    min[j] = 0;
    int temp = arr[0][j];
    for (int i = 1; i < row; i++)
    {
      if (arr[i][j] < min[j])
      {
        min[j] = i;
        temp = arr[i][j];
      }
    }
  }

  int count = 0;
  for (int i = 0; i < row; i++)
  {
    if (min[max[i]] == i)
    {
      count++;
      printf("[%d][%d]=%d\n", i, max[i], arr[i][max[i]]);
    }
  }
  if (!count)
    puts("No saddle point.");
}

int main()
{
  int row, col;
  puts("Please input the row and col:");
  scanf("%d%d", &row, &col);
  puts("Please input the matrix:");
  for (int i = 0; i < row; i++)
  {
    for (int j = 0; j < col; j++)
      scanf("%d", &arr[i][j]);
  }
  saddlePoint(arr, row, col);
  return 0;
}