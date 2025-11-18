/**
 * 输出杨辉三角形前10行。格式如下：
 * 1
 * 1 1
 * 1 2 1
 * 1 3 3 1
 * 1 4 6 4 1
 * ……
 */
#include <stdio.h>
#include <string.h>

int triangle[10][10];

int main()
{
  memset(triangle, 0, sizeof(triangle));
  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 10; j++)
    {
      if (j == 0)
      {
        triangle[i][j] = 1;
      }
      else if (i == 0 || j > i)
      {
        break;
      }
      else
      {
        triangle[i][j] = triangle[i - 1][j - 1] + triangle[i - 1][j];
      }
    }
  }

  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j <= i; j++)
    {
      printf("%-4d", triangle[i][j]);
    }
    printf("\n");
  }
  return 0;
}