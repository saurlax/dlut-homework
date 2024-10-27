/**  
 * 输入一个日期
 * （1）编写函数判断其合理性；要求该函数调用闰年函数
 * （2）调用函数输出该日期是当年的第几天
 */
#include <stdio.h>

int is_leap_year(int year)
{
  if (year % 400 == 0)
    return 1;
  if (year % 100 == 0)
    return 0;
  if (year % 4 == 0)
    return 1;
  return 0;
}

int valid_date(int year, int month, int day)
{
  if (year < 0 || month < 1 || month > 12 || day < 1 || day > 31)
    return 0;
  if (month == 2)
  {
    if (day > 29)
      return 0;
    if (day == 29 && !is_leap_year(year))
      return 0;
  }
  if (month == 4 || month == 6 || month == 9 || month == 11)
  {
    if (day > 30)
      return 0;
  }
  return 1;
}

int main()
{
  int year, month, day;
  printf("Please input a date (yyyy/mm/dd): ");
  scanf("%d/%d/%d", &year, &month, &day);
  if (!valid_date(year, month, day))
  {
    puts("Invalid date!");
  }
  else
  {
    int days = 0;
    for (int i = 1; i < month; i++)
    {
      if (i == 2)
      {
        days += 28 + is_leap_year(year);
      }
      else if (i == 4 || i == 6 || i == 9 || i == 11)
      {
        days += 30;
      }
      else
      {
        days += 31;
      }
    }
    days += day;
    printf("It's the %dth day of the year\n", days);
  }
  return 0;
}