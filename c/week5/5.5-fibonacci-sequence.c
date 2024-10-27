/**  
 * 已知斐波那契数列
 * Fibonacci数列：0，1，1，2，3，5，8，13，21，34，...
 * f0=0
 * f1=1
 * fn=fn-1+fn-2(n>=2)
 * 输出数列的前20项，每行输出8个数
 */
#include <stdio.h>

int main()
{
  int f0 = 0, f1 = 1, fn;
  printf("%d\t%d\t", f0, f1);
  for (int i = 3; i <= 20; i++)
  {
    fn = f0 + f1;
    printf("%d\t", fn);
    if (i % 8 == 0)
    {
      printf("\n");
    }
    f0 = f1;
    f1 = fn;
  }
  return 0;
}