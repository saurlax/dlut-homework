/**
 * n个学生参加期末考试（n值由输入得到，定义动态数组存储学生的n个成绩int），并按照降序排列学生成绩。
 * 考察动态空间分配
 */
#include <stdio.h>
#include <stdlib.h>

int main()
{
  int n;
  puts("Please input the number of students:");
  scanf("%d", &n);
  int *grade = (int *)malloc(n * sizeof(int));
  puts("Please input the grades of students:");
  for (int i = 0; i < n; i++)
    scanf("%d", &grade[i]);

  for (int i = 0; i < n; i++)
  {
    int max = i;
    for (int j = i + 1; j < n; j++)
      if (grade[j] > grade[max])
        max = j;
    if (max != i)
    {
      int temp = grade[i];
      grade[i] = grade[max];
      grade[max] = temp;
    }
  }

  for (int i = 0; i < n; i++)
    printf("%d ", grade[i]);

  free(grade);
  return 0;
}