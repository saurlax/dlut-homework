/**
 * 编写函数，通过指针比较两个字符串。两个字符串自左向右逐个字符相比（按 ASCII 值大小相比较），直到出现不同的字符或遇 \0 为止。
 * 注意不要用关系运算符比较两个指针，在比地址。
 * 函数原型 int strCmp (char *pc1 , char *pc2); //pc1大则返回正数，pc2大则返回负数,完全相同则返回0
 */
#include <stdio.h>

int strCmp(char *pc1, char *pc2)
{
  while (*pc1 == *pc2 && *pc1 != '\0' && *pc2 != '\0')
  {
    pc1++;
    pc2++;
  }
  return *pc1 - *pc2;
}

int main()
{
  char str1[100], str2[100];
  puts("Please input str1:");
  scanf("%s", str1);
  puts("Please input str2:");
  scanf("%s", str2);
  printf("The result is %d\n", strCmp(str1, str2));
  return 0;
}