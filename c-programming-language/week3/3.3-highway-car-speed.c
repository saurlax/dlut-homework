/**  
 * 汽车在高速公路上匀速行驶。沿途都有距离上路地点的里程标志，已知开始和结束的里程，
 * 分别输入上路时间和下路时间（时、分、秒），假设汽车上路和下路的时间在同一天。计算
 * 汽车在该段形式的平均速度，并以“公里数/每小时”的形式输出平均速度。程序的运行界面
 * 如图所示。
 */
#include <stdio.h>

int main()
{
  int mile1, mile2, hour1, min1, sec1, hour2, min2, sec2;
  int mile, hour, min, sec;
  double time, speed;
  printf("所有输入数据为整数\n");
  printf("开始里程？\n");
  scanf("%d", &mile1);
  printf("开始时间（时，分，秒）？\n");
  scanf("%d%d%d", &hour1, &min1, &sec1);
  printf("结束里程？\n");
  scanf("%d", &mile2);
  printf("结束时间（时，分，秒）？\n");
  scanf("%d%d%d", &hour2, &min2, &sec2);

  mile = mile2 - mile1;
  sec = sec2 - sec1;
  min = min2 - min1;
  hour = hour2 - hour1;
  if (sec < 0)
  {
    min--;
    sec += 60;
  }
  if (sec >= 60)
  {
    min++;
    sec -= 60;
  }
  if (min < 0)
  {
    hour--;
    min += 60;
  }
  if (min >= 60)
  {
    hour++;
    min -= 60;
  }
  time = hour + min / 60.0 + sec / 3600.0;
  speed = mile / time;

  printf("汽车行驶了%d公里\n", mile);
  printf("消耗时间%d小时%02d分%02d秒\n", hour, min, sec);
  printf("平均速度：%.4lf公里/小时\n", speed);

  return 0;
}
