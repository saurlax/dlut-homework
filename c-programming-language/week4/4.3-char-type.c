/**  
 * 字符型数据可以简单分为数字、大写字母、小写字母及其他字符4类。从键盘输入一个字符，输出它的类型。
 */
#include <stdio.h>

int main()
{
  char c = getchar();
  if (c >= '0' && c <= '9')
  {
    puts("number");
  }
  else if (c >= 'A' && c <= 'Z')
  {
    puts("capital letter");
  }
  else if (c >= 'a' && c <= 'z')
  {
    puts("lowercase letter");
  }
  else
  {
    puts("other");
  }
  return 0;
}
