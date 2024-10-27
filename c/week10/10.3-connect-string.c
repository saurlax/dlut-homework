/**
 * 编写函数实现字符串的连接, 将pStr2连接到pStr1后，并存储在pStr1指向的数组中
 * char* strConnect(char *pStr1, int n, char *pStr2);
 * 注意检测pStr1指向的长度为n的字符数组有足够的空间存放连接后的字符串，若不能存放连接后完整的新串，则只存满n-1个字符，并以\0结尾，函数返回pStr1。
 */
#include <stdio.h>

char *strConnect(char *pStr1, int n, char *pStr2)
{
  int i = 0, j = 0;
  while (pStr1[i] != '\0')
    i++;
  while (pStr2[j] != '\0' && i < n)
  {
    pStr1[i++] = pStr2[j++];
  }
  pStr1[i] = '\0';
  return pStr1;
}

int main()
{
  char str1[100], str2[100];
  puts("Please input str1:");
  scanf("%s", str1);
  puts("Please input str2:");
  scanf("%s", str2);
  strConnect(str1, 100, str2);
  puts(str1);
  return 0;
}