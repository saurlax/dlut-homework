/**  
 * Write a program that converts time in minutes to time in hours and minutes. Use
 * #define or const to create a symbolic constant for 60. Use a while loop to allow the
 * user to enter values repeatedly and terminate the loop if a value for the time of 0 or less
 * is entered.
 */
#include <stdio.h>

const int MINS_PRE_HOUR = 60;

int main()
{
  int time;
  printf("Input a time in minutes and I can convert it to time in hours and minutes for u:\n");
  while (1)
  {
    scanf("%d", &time);
    if (time <= 0)
    {
      break;
    }
    printf("%d:%02d\n", time / MINS_PRE_HOUR, time % MINS_PRE_HOUR);
  }
  return 0;
}
