/**  
 * 输出 n 到 m 内的所有素数，并统计素数的个数。
 */
#include <stdio.h>
#include <string.h>
#define max 1 << 20

int len = 0;
int primes[max];
unsigned char nums[max];

int main()
{
  int m, n;
  printf("Please input the range of prime numbers you want: ");
  scanf("%d%d", &m, &n);
  if (m > n)
  {
    puts("m should be less than n");
    return 0;
  }
  memset(nums, 1, sizeof(nums));
  for (int i = 2; i <= n; i++)
  {
    if (nums[i])
    {
      primes[len++] = i;
    }
    for (int j = 0; j < len; j++)
    {
      long long num = (long long)i * primes[j];
      if (num > n)
      {
        break;
      }
      nums[num] = 0;
      if (i % primes[j] == 0)
      {
        break;
      }
    }
  }

  int start;
  for (int i = 0; i < len; i++)
  {
    if (primes[i] >= m)
    {
      start = i;
      break;
    }
  }

  for (int i = start; i < len; i++)
  {
    printf("%d\n", primes[i]);
  }
  printf("There are %d prime numbers between %d to %d\n", len - start, m, n);
  return 0;
}