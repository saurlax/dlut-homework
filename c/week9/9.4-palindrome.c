/**
 * abba是回文，level是回文。
 * 编写一个函数，判断一个单词（字符串）是否是“回文”，请用函数递归实现。
 * 思路：如果一个字符串的首尾字符相同，且去掉首尾的子串也是回文，那么这个字符串就是回文字符串。
 * 函数原型：int isPalindrome(char *s, int size);
 * 其中s为字符串首地址，size为字符个数。
 */
#include <stdio.h>
#include <string.h>

int isPalindrome(char *str, int n)
{
  if (n <= 1)
    return 1;
  if (str[0] == str[n - 1])
    return isPalindrome(str + 1, n - 2);
  else
    return 0;
}

int main()
{
  char str[100];
  puts("Please input a string:");
  scanf("%s", str);
  if (isPalindrome(str, strlen(str)))
    puts("Is palindrome");
  else
    puts("Not palindrome");
}