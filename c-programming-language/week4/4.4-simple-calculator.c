/**  
 * 设计简单的计算器程序。要求根据用户输入的表达式进行计算：
 * 操作数1 运算符op 操作数2
 * 指定的运算符为+ - * /
 * 操作数均为浮点数。
 */
#include <stdio.h>

int main()
{
  double a, b;
  char op;

  while (1)
  {
    scanf("%lf %c %lf", &a, &op, &b);
    switch (op)
    {
    case '+':
      printf("%lf\n", a + b);
      break;
    case '-':
      printf("%lf\n", a - b);
      break;
    case '*':
      printf("%lf\n", a * b);
      break;
    case '/':
      if (b == 0)
      {
        printf("error\n");
      }
      else
      {
        printf("%lf\n", a / b);
      }
      break;
    }
  }
  return 0;
}
