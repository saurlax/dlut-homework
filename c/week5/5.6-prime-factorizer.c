/**  
 * 输入一个正整数，如果该数不是素数，则将其分解质因数，输出所有的因子。例如：输入90，打印出90=2*3*3*5。
 */
#include <stdio.h>
#include <math.h>

int main()
{
  int n;
  while (1)
  {
    printf("Please input a positive integer: ");
    scanf("%d", &n);
    if (n < 3)
    {
      printf("Invalid input\n");
      break;
    }

    int isPrime = 1;
    int k = sqrt(n);
    for (int i = 2; i < k; i++)
    {
      if (n % i == 0)
      {
        isPrime = 0;
        break;
      }
    }

    if (isPrime)
    {
      printf("%d is a prime number\n", n);
      continue;
    }

    printf("%d=", n);
    int i = 2;
    while (n != 1)
    {
      if (n % i == 0)
      {
        printf("%d", i);
        n /= i;
        if (n != 1)
        {
          printf("*");
        }
      }
      else
      {
        i++;
      }
    }
    printf("\n");
  }
  return 0;
}