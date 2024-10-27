/**
 * 合数（Composite number），即不是素数的正整数。例如：4, 6, 8, 9, 10等。
 * 输入一个正整数number（不超过20），调用getCompos函数返回“1到number之间”所有合数的个数，并获得这些合数。函数原型如下：
 * int getCompos(int number, int *compos);
 * 其中number为输入的正整数，compos为合数数组的首地址，该函数返回合数的个数。
 * 注意应在主函数中定义一个存放合数的数组（长度为20），然后将首地址传给getCompos函数。
 */
#include <stdio.h>

int getCompos(int number, int *compos)
{
  int count = 0;
  for (int i = 2; i <= number; i++)
  {
    int flag = 0;
    for (int j = 2; j < i; j++)
    {
      if (i % j == 0)
      {
        flag = 1;
        break;
      }
    }
    if (flag)
    {
      compos[count++] = i;
    }
  }
  return count;
}

int main()
{
  int number, compos[20];
  puts("Please input a number:");
  scanf("%d", &number);
  if (number > 20)
  {
    puts("The number should be less than 20.");
    return 0;
  }
  printf("The number of compos is %d.\n", getCompos(number, compos));
  printf("The compos are: ");
  for (int i = 0; i < getCompos(number, compos); i++)
  {
    printf("%d ", compos[i]);
  }
  puts("");
  return 0;
}