/**
 * 编写函数实现在字符串pStr1中查找子串pStr2
 * char *subString( char* pStr1,  char* pStr2); //返回pStr2在pStr1中第一次出现的地址。没找到返回NULL
 */
#include <stdio.h>

char *subString(char *pStr1, char *pStr2)
{
  char *p1 = pStr1;
  char *p2 = pStr2;
  for (; *p1 != '\0'; p1++)
  {
    if (*p1 == *p2)
    {
      char *ans = p1;
      for (; *p2 != '\0'; p1++, p2++)
        if (*p1 != *p2)
          break;
      if (*p2 == '\0')
      {
        return ans;
      }
      else
      {
        p1 = ans;
        p2 = pStr2;
      }
    }
  }
  return NULL;
}

int main()
{
  char str1[100], str2[100];
  puts("Please input str1:");
  scanf("%s", str1);
  puts("Please input str2:");
  scanf("%s", str2);
  char *p = subString(str1, str2);
  if (p == NULL)
    puts("Not found");
  else
    puts(p);
  return 0;
}