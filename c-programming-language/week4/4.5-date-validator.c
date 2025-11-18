/**  
 * 编写程序判断日期是否有效。用户输入日期（年、月、日），输出相应的判断结果。
 * 提示：年year为正整数，月month为1-12的整数，日day为0-maxDay的整数，其中maxDay的值
 * 取决于该日期所在的年份（是否为闰年）和月份（大月、小月或2月）
 */
#include <stdio.h>

int main()
{
  int year, month, day, maxDay, vaild = 1;

  while (1)
  {
    printf("Enter date (year month day): ");
    scanf("%d%d%d", &year, &month, &day);
    if (year < 0)
    {
      puts("Year should be positive");
      vaild = 0;
    }
    if (month < 0 || month > 12)
    {
      puts("Month should be in 1~12");
      vaild = 0;
    }
    if (month == 2)
    {
      if ((year % 4 == 0 && year & 100 != 0) || year % 400 == 0)
      {
        maxDay = 29;
      }
      else
      {
        maxDay = 28;
      }
    }
    else
    {
      switch (month)
      {
      case 4:
      case 6:
      case 9:
      case 11:
        maxDay = 30;
        break;
      default:
        maxDay = 31;
        break;
      }
    }
    if (day < 0 || day > maxDay)
    {
      printf("Day should be in 1~%d\n", maxDay);
      vaild = 0;
    }
    if (vaild)
    {
      puts("Correct date");
    }
  }
  return 0;
}
