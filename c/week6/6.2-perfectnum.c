/**  
 * 一个整数的所有因子（包括1，但不包括本身）之和等于该数，则该数称为完数。
 * 例如6是一个完数，因为6=1+2+3。编写一个isPerfectnum函数，
 * 判断参数number是否为完数。利用该函数判断并打印1到1000之间的所有完数。
 */
#include <stdio.h>

int isPerfectnum(int num)
{
  int sum = 0;
  for (int i = 1; i < num; i++)
  {
    if (num % i == 0)
    {
      sum += i;
    }
  }
  return sum == num;
}

int main()
{
  for (int i = 1; i <= 1000; i++)
  {
    if (isPerfectnum(i))
    {
      printf("%d\n", i);
    }
  }
  return 0;
}