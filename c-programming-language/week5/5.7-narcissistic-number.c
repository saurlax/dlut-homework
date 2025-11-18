/**  
 * 输出所有的三位水仙花数，每输出3个数就换行。
 * “水仙花数”是指一个三位数，其各位数字的立方和等于该数本身，
 * 如 153=1*1*1+5*5*5+3*3*3
 */
#include <stdio.h>

int main()
{
  int count = 0;
  for (int i = 100; i < 1000; i++)
  {
    int a = i / 100;
    int b = i / 10 % 10;
    int c = i % 10;
    if (a * a * a + b * b * b + c * c * c == i)
    {
      printf("%d\t", i);
      if (++count % 3 == 0)
      {
        printf("\n");
      }
    }
  }
  return 0;
}