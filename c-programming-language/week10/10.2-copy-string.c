/**
 * 编写函数，用指针移动法实现字符串的复制，不能使用库函数strcpy。
 * char* strCopy(char* pStr1, int n, char* pStr2);
 * 注意该函数需要检测pStr1指向的长度为n的字符数组有足够的空间存放字符串pStr2，若存不下完整的字符串pStr2，则只复制n-1个字符，并以\0结尾，函数返回pStr1。
 */
#include <stdio.h>

char *strCopy(char *pStr1, int n, char *pStr2)
{
  int i = 0;
  while (i < n && pStr2[i] != '\0')
  {
    pStr1[i] = pStr2[i];
    i++;
  }
  pStr1[i] = '\0';
  return pStr1;
}

int main()
{
  char str1[100], str2[100];
  puts("Please input a string:");
  scanf("%s", str1);
  strCopy(str2, 100, str1);
  puts(str2);
  return 0;
}