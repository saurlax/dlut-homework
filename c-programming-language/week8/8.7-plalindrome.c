/**
 * 输入一个长度不超过50的字符串，判断该字符串是否为回文。（字符串）
 * 简单回文：1234321， aabccxccbaa
 * 去除标点和空格的复杂回文： Was it a bar or a bat I saw?
 * A man, a plan, a canal, Panama.
 */
#include <stdio.h>

int main()
{
  char str[50], c;
  int len = 0;
  puts("Please input a string:");
  while (c = getchar(), c != '\n')
  {
    if (c >= 'A' && c <= 'Z')
      str[len++] = c + 32;
    if (c >= 'a' && c <= 'z' || c >= '0' && c <= '9')
      str[len++] = c;
  }

  for (int i = 0; i < len / 2; i++)
  {
    if (str[i] != str[len - i - 1])
    {
      puts("No palindrome.");
      return 0;
    }
  }
  puts("Palindrome.");
  return 0;
}