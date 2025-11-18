/**
 * 输入一个数字字符串，将其转换成一个整数，只考虑十进制的情况。（字符串与函数）
 * （1）定义 valid() 函数检测字符串是否是合法数字（注意负数的情况）
 * （2）定义 toNumber() 函数将字符串转化为整数
 */
#include <stdio.h>

int vaild(char *str, int len)
{
  int i = str[0] == '-';
  for (; i < len; i++)
  {
    if (str[i] < '0' || str[i] > '9')
      return 0;
  }
  return 1;
}

long long toNumber(char *str, int len)
{
  long long num = 0;
  int i = str[0] == '-';
  for (; i < len; i++)
  {
    num = num * 10 + str[i] - '0';
  }
  return str[0] == '-' ? -num : num;
}

int main()
{
  char str[100], c;
  int len = 0;
  puts("Please input a string:");
  while (c = getchar(), c != '\n')
  {
    str[len++] = c;
  }
  if (!vaild(str, len))
  {
    puts("Invalid input.");
    return 0;
  }
  printf("%lld\n", toNumber(str, len));
  return 0;
}