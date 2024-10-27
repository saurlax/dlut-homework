/**  
 * 编写计算组合数C(n,m)的函数。
 * 主函数中输入n和m的值（n>m），调用函数输出组合数n!/(m!(n-m)!)。
 */
#include <stdio.h>

int combination(int n, int m)
{
  int ans = 1;
  for (int i = 0; i < m; i++)
  {
    ans *= n - i;
    ans /= i + 1;
  }
  return ans;
}

int main()
{
  int n, m;
  printf("Please input n and m (n>m): ");
  scanf("%d%d", &n, &m);
  printf("C(%d,%d) = %d\n", n, m, combination(n, m));
  return 0;
}