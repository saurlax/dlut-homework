/**
 * 编写函数将一个不大于1000的十进制整数转换成十六进制数，并输出（采用递归算法实现）。
 */
#include <stdio.h>

void toHex(int n)
{
  if (n == 0)
  {
    putchar('0');
    return;
  }

  int t = n % 16;
  putchar(t < 10 ? t + '0' : t - 10 + 'A');

  n /= 16;
  if (n)
    toHex(n);
}

int main()
{
  int num;

  puts("Please input a number: ");
  scanf("%d", &num);

  if (num > 1000)
  {
    puts("The number should be less than 1000.");
    return 0;
  }
  toHex(num);

  return 0;
}