/**  
 * 编写程序实现华氏温度和摄氏温度的转换。输入一个华氏温度F，要求输出摄氏温度C。
 * 输出结果要有文字说明，取小数点后4位数字。转换公式为：C=5*(F-32)/9
 */
#include <stdio.h>

int main()
{
  double temp;
  printf("please enter a fahrenheit temperature (F): ");
  scanf("%lf", &temp);
  printf("result in celsius temperature (C) is: %.4lf", 5 * (temp - 32) / 9);
  return 0;
}
