/**
 * 输入两个1970年-3000年之间的日期（年、月、日），输出两个日期相差多少天。需要完成以下函数
 * （1）闰年判断函数
 * （2）日期合法性检测函数
 * （3）日期是一年中第几天函数
 * （4）两日期相差几天函数
 * 如需要还可以补充其他函数。利用数组优化算法。
 */
#include <stdio.h>

int isLeapYear(int year)
{
  return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

int isDateValid(int date[3])
{
  if (date[0] < 1970 || date[0] > 3000)
    return 0;
  if (date[1] < 1 || date[1] > 12)
    return 0;
  if (date[2] < 1 || date[2] > 31)
    return 0;
  if (date[1] == 2)
    return isLeapYear(date[0]) ? date[2] <= 29 : date[2] <= 28;
  if (date[1] == 4 || date[1] == 6 || date[1] == 9 || date[1] == 11)
    return date[2] <= 30;
  return 1;
}

int dayOfYear(int date[3])
{
  int days = 0;
  for (int i = 1; i < date[1]; i++)
  {
    switch (i)
    {
    case 2:
      days += isLeapYear(date[0]) ? 29 : 28;
      break;
    case 4:
    case 6:
    case 9:
    case 11:
      days += 30;
      break;
    default:
      days += 31;
      break;
    }
  }
  days += date[2];
  return days;
}

int daysBetween(int date1[3], int date2[3])
{
  int days = 0;
  if (date1[0] <= date2[0])
  {
    for (int i = date1[0]; i < date2[0]; i++)
    {
      days += isLeapYear(i) ? 366 : 365;
    }
    days += dayOfYear(date2) - dayOfYear(date1);
  }
  else
  {
    for (int i = date2[0]; i < date1[0]; i++)
    {
      days -= isLeapYear(i) ? 366 : 365;
    }
    days -= dayOfYear(date1) - dayOfYear(date2);
  }
  return days;
}

int main()
{
  int date1[3], date2[3];

  puts("Please input the first date (year, month, day):");
  scanf("%d%d%d", &date1[0], &date1[1], &date1[2]);
  puts("Please input the second date (year, month, day):");
  scanf("%d%d%d", &date2[0], &date2[1], &date2[2]);
  if (!isDateValid(date1) || !isDateValid(date2))
  {
    puts("Invalid date!");
    return 0;
  }

  printf("Days between: %d\n", daysBetween(date1, date2));

  return 0;
}