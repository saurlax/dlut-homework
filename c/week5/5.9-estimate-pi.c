/**  
 * 求“派”的近似值，利用如下级数公式估算“派”，要求误差小于10的-6次方。
 * pi/2 = (2*2)/(1*3) * (4*4)/(3*5) * ... * (2n*2n)/[(2n-1)*(2n+1)]
 */
#include <stdio.h>

int main()
{
  int n = 1;
  double pi = 2;
  double lastPi = 0;
  while (pi - lastPi > 1e-12)
  {
    lastPi = pi;
    // don't directly use (2*n*2*n)/((2*n-1)*(2*n+1)) bacause it may less than 1
    pi *= 4.0 * n * n / (2 * n - 1) / (2 * n + 1);
    n++;
  }
  printf("pi=%.6lf\n", pi);
  return 0;
}