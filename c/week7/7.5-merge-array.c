/**
 * 将两个从小到大有序整型数组a和b合并出一个有序整数数组c。
 */
#include <stdio.h>

int a[10000], b[10000], c[20000];
int alen = 0, blen = 0, clen = 0;

int main()
{
  puts("Please input array a:");
  do
  {
    scanf("%d", &a[alen++]);
  } while (getchar() != '\n');

  puts("Please input array b:");
  do
  {
    scanf("%d", &b[blen++]);
  } while (getchar() != '\n');

  int i = 0, j = 0;
  while (i < alen || j < blen)
  {
    if (i == alen)
    {
      puts("b++ (a full)");
      c[clen++] = b[j++];
      continue;
    }
    if (j == blen)
    {
      c[clen++] = a[i++];
      continue;
    }
    if (a[i] < b[j])
    {
      c[clen++] = a[i++];
    }
    else
    {
      c[clen++] = b[j++];
    }
  }

  puts("The merged array is:");
  for (int i = 0; i < clen; i++)
  {
    printf("%d ", c[i]);
  }
  return 0;
}