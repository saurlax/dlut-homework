/**
 * 用筛法计算100之内的素数。
 * 算法概述：先把N个自然数按次序排列起来。1不是质数，也不是合数，要划去。
 * 第二个数2是质数留下来，而把2后面所有能被2整除的数都划去。2后面第一个
 * 没划去的数是3，把3留下，再把3后面所有能被3整除的数都划去。3后面第一个
 * 没划去的数是5，把5留下，再把5后面所有能被5整除的数都划去。这样一直做
 * 下去，就会把不超过N的全部合数都筛掉，留下的就是不超过N的全部质数。
 */
#include <stdio.h>
#include <string.h>
#define max 100

int len = 0;
int primes[max];
unsigned char nums[max];

int main()
{
  memset(nums, 1, sizeof(nums));
  for (int i = 2; i < max; i++)
  {
    if (nums[i])
    {
      primes[len++] = i;
    }
    for (int j = 0; j < len; j++)
    {
      long long num = (long long)i * primes[j];
      if (num >= max)
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

  for (int i = 0; i < len; i++)
  {
    printf("%d ", primes[i]);
  }
  return 0;
}