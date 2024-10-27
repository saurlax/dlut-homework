/**  
 * Write a program that asks the user to enter a height in centimeters and then displays the
 * height in centimeters and in feet and inches. Fractional centimeters and inches should
 * be allowed, and the program should allow the user to continue entering heights until a
 * nonpositive value is entered. A sample run should look like this:
 * 
 * Enter a height in centimeters: 182
 * 182.0 cm = 5 feet, 11.7 inches
 * Enter a height in centimeters (<=0 to quit): 168.7
 * 168.0 cm = 5 feet, 6.4inches
 * Enter a height in centimeters (<=0 to quit): 0
 * bye
 */
#include <stdio.h>

double cm, inch;
int feet, show;

int main()
{
  while (1)
  {
    if (show)
    {
      printf("Enter a height in centimeters (<=0 to quit): ");
    }
    else
    {
      printf("Enter a height in centimeters: ");
    }
    scanf("%lf", &cm);
    if (cm <= 0)
    {
      break;
    }

    inch = cm / 2.54;
    feet = inch / 12;
    inch = (int)inch % 12 + inch - (int)inch;
    printf("%.1lf cm = %d feet, %.1lf inches\n", cm, feet, inch);
    show = 1;
  }
  printf("bye\n");
  return 0;
}
