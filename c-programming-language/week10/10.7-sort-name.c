/**
 * 有5个人的名字，全部由小写英文字母组成，且长度不超过9个字符。请编写程序按照字典顺序输出这5个名字。可以使用string.h提供的字符串函数。
 * 注：此题有两种不同的思路：一是使用strcpy将5个字符串重新存储排列；二是不改变字符串的存储，仅改变指针的指向。
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *p[5];

int main()
{
  puts("Please input 5 names:");
  for (int i = 0; i < 5; i++)
  {
    char *name = (char *)malloc(sizeof(char) * 10);
    scanf("%s", name);
    p[i] = name;
  }

  for (int i = 0; i < 5; i++)
  {
    printf("%s\n", p[i]);
  }

  for (int i = 0; i < 5; i++)
  {
    int min = i;
    for (int j = i + 1; j < 5; j++)
    {
      if (strcmp(p[min], p[j]) > 0)
      {
        min = j;
      }
    }
    if (min != i)
    {
      char *temp = p[i];
      p[i] = p[min];
      p[min] = temp;
    }
  }

  puts("The names in dictionary order are:");
  for (int i = 0; i < 5; i++)
  {
    printf("%s\n", p[i]);
  }
  return 0;
}