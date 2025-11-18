/**  
 * 编写程序验证歌德巴赫猜想：一个不小于6的偶数可以表示成两个素数之和。
 * 如6=3+3，8=3+5，10=3+7，...
 * 在主函数中输入一个大于6的偶数n，例如34，
 * 输出如下形式：34=3+31。输出不满足要求的数则重新输入。
 */
#include <stdio.h>

int is_prime(int x)
{
  if (x < 2)
  {
    return 0;
  }
  if (x == 2)
  {
    return 1;
  }
  if (x % 2 == 0)
  {
    return 0;
  }
  for (int i = 3; i * i <= x; i += 2)
  {
    if (x % i == 0)
    {
      return 0;
    }
  }
  return 1;
}

int main()
{
  int n;
  while (1)
  {
    printf("Please input an even number greater than 6: ");
    scanf("%d", &n);
    if (n <= 6 || n % 2 != 0)
    {
      puts("Invalid input");
      continue;
    }
    for (int i = 0; i < n / 2; i++)
    {
      if (is_prime(i) && is_prime(n - i))
      {
        printf("%d=%d+%d\n", n, i, n - i);
        break;
      }
    }
  }
  return 0;
}