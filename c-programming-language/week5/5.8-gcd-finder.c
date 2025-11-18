/**  
 * 求m和n的最大公约数。分别使用“穷举法”和“辗转相除法”实现。
 * 1 穷举法：列出所有可能出现的情况，并一一测试。
 * 即在小于等于两数小值的所有整数中寻找最大公约数。
 * 2 辗转相除法：最大公约数为 n 与 m%n 的最大公约数；
 * 用 n 和 m%n 替换原来的 m 与 n；直到 n = 0，
 * 当前 m 的值即为所求的最大公约数。例如 m=24，n=9时：
 * (1) 24和9的最大公约数等于 (24%9)=6 和9的最大公约数
 * (2) 9和6的最大公约数等于 (9%6)=3 和6的最大公约数
 * (3) 6和3的最大公约数等于 (6%3)=0 和3的最大公约数
 * 因此 24和9的最大公约数等于3
 */
#include <stdio.h>

int exhaustion(int m, int n)
{
  int min = m < n ? m : n;
  for (int i = min; i > 0; i--)
  {
    if (m % i == 0 && n % i == 0)
    {
      return i;
    }
  }
  return 1;
}
int division(int m, int n)
{
  int a = 0;
  while (n)
  {
    a = m % n;
    m = n;
    n = a;
  }
  return m;
}

int main()
{
  int m, n;
  while (1)
  {
    printf("Please enter two integers: ");
    scanf("%d%d", &m, &n);
    printf("Exhaustive search method: %d\n", exhaustion(m, n));
    printf("Euclidean algorithm: %d\n", division(m, n));
  }
  return 0;
}