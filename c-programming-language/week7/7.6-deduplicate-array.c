/**
 * 把一个数组中重复元素去掉, 即不在数组中存储, 输出数组中剩余的元素及其数目。
 * 例如，对于数组a[12]={1,1,2,7,3,2,3,4,5,8,7,4}，输出剩余7个数据：1,2,7,3,4,5,8。
 */
#include <stdio.h>

int array[10000];
int len = 0;

int main()
{
  puts("Please input array:");
  do
  {
    scanf("%d", &array[len++]);
  } while (getchar() != '\n');

  for (int i = 0; i < len; i++)
  {
    for (int j = i + 1; j < len; j++)
    {
      if (array[i] == array[j])
      {
        for (int k = j; k < len - 1; k++)
        {
          array[k] = array[k + 1];
        }
        len--;
        j--;
      }
    }
  }

  puts("The deduplicated array is:");
  for (int i = 0; i < len; i++)
  {
    printf("%d ", array[i]);
  }
  return 0;
}