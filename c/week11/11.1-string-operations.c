/**
 * 有字符串数组：
 * char ss[6][10]={"aaaaaa","bbb","abcdef", "aaaaaa","AA","AAAbbb"};
 * 使用标准库函数完成以下操作
 * （1）将ss存储的字符串存入数组sn（char sn[6][7]），注意完全相同的字符串只存入一次（即：aaaaaa第二次无法存入sn）。
 * （2）将sn中第一次出现的长度最短字符串连接上后缀's'（不会超出长度限制）
 * （3）输出sn所有字符串
 * 练习使用string.h提供的字符串操作标准库函数
 */
#include <stdio.h>
#include <string.h>

int main()
{
  char ss[6][10] = {"aaaaaa", "bbb", "abcdef", "aaaaaa", "AA", "AAAbbb"};
  char sn[6][7];
  int len = 0;
  for (int i = 0; i < 6; i++)
  {
    int flag = 1;
    for (int j = 0; j < len; j++)
    {
      if (!strcmp(ss[i], sn[j]))
      {
        flag = 0;
        break;
      }
    }
    if (flag)
    {
      strcpy(sn[len++], ss[i]);
    }
  }

  int min = 0;
  for (int i = 0; i < len; i++)
  {
    if (strlen(sn[i]) < strlen(sn[min]))
    {
      min = i;
    }
  }
  strcat(sn[min], "s");

  for (int i = 0; i < len; i++)
  {
    printf("%s\n", sn[i]);
  }
  return 0;
}