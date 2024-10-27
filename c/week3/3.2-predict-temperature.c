/**  
 * 编写程序预测断电后冰箱的温度。断电后经过一段时间，温度保持情况由
 * 如下公式决定：T=4t^2/(t+2)-20
 * 其中，t为断电后经过的时间（小时），T为温度（℃）。经过若干小时后
 * 冰箱温度接近室温并保持不变. 假设室温为18℃. 程序提示用户输入时间，
 * 它以整数小时和分钟表示，需要将其转换为浮点型的小时数。输出此时的
 * 温度值（小数点后2位有效数字）。
 */
#include <stdio.h>

int main()
{
  int hour, min;
  double time, temp;
  printf("Please enter the time elapsed since the power off: ");
  scanf("%d:%d", &hour, &min);
  time = hour + min / 60.0;
  temp = 4 * time * time / (time + 2) - 20;
  if (temp > 18)
  {
    temp = 18;
  }
  printf("The current temperature of the refrigerator is %.2lf", temp);
  return 0;
}
