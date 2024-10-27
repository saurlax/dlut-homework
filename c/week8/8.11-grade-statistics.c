/**
 * 统计M名学生的N门课的成绩, 分别实现以下功能：
 * （1）输入M个学生的名字和成绩
 * （2）double ave(int grade[][N], int m); 计算某名同学的平均分
 * （3）double pass(int grade[][N], int n); 统计某门课程的及格率。
 * （4）void print(int grade[][N], char name[][10])
 * // char name[][10] 是存储名字的字符串数组
 * 按如下格式输出所有成绩。（无需输出横竖表格线）姓名使用拼音即可。
 *           赵    钱    孙    李    张    及格率
 * Math      80    61    50    85    76    0.8
 * C         75    65    56    87    50    0.6
 * English   92    71    70    90    65    1.0
 * Average
 * 要求: 不能把成绩数组定义为全局变量。
 */
#include <stdio.h>
#define M 5
#define N 3

double ave(int grade[][N], int m)
{
  int sum = 0;
  for (int i = 0; i < N; i++)
  {
    sum += grade[m][i];
  }
  return (double)sum / N;
}

double pass(int grade[][N], int n)
{
  int count = 0;
  for (int i = 0; i < M; i++)
  {
    if (grade[i][n] >= 60)
      count++;
  }
  return (double)count / M;
}

void print(int grade[][N], char name[][10])
{
  putchar('\t');
  for (int i = 0; i < M; i++)
  {
    printf("%s\t", name[i]);
  }
  puts("Pass rate");

  char subject[][10] = {"Math", "C", "English"};
  for (int n = 0; n < N; n++)
  {
    printf("%s\t", subject[n]);
    for (int i = 0; i < M; i++)
    {
      printf("%d\t", grade[i][n]);
    }
    printf("%.1lf\n", pass(grade, n));
  }

  printf("Average\t");
  for (int i = 0; i < M; i++)
  {
    printf("%.1lf\t", ave(grade, i));
  }
}

int main()
{
  int grade[M][N] = {
      {80, 75, 92},
      {61, 65, 71},
      {50, 56, 70},
      {85, 87, 90},
      {76, 50, 65}};
  char names[][10] = {"Zhao", "Qian", "Sun", "Li", "Zhang"};

  print(grade, names);
  return 0;
}
