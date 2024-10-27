/**  
 * Write a program that asks the user to enter the number of miles traveled and the number
 * of gallons of gasoline consumed. It should then calculate and display the miles-per-gallon
 * value, showing one place to the right of the decimal. Next, using the fact that one gallon
 * is about 3.785 liters and one mile is about 1.609 kilometers, it should convert the mile-
 * per-gallon value to a liters-per-100-km value, the usual European way of expressing fuel
 * consumption, and display the result, showing one place to the right of the decimal. Note
 * that the U. S. scheme measures the distance traveled per amount of fuel (higher is better),
 * whereas the European scheme measures the amount of fuel per distance (lower is better).
 * Use symbolic constants (using const or #define) for the two converslon factors.
 */
#include <stdio.h>

const double gal2l = 3.758;
const double mil2km = 1.609;
double dist, gas;

int main()
{
  printf("Please enter distance traveled (mil) and gas consumed (gal): ");
  scanf("%lf%lf", &dist, &gas);
  printf("Unit gas consumption (us std.): %.1lf mil/gal\n", dist / gas);
  dist *= mil2km;
  gas *= gal2l;
  printf("Unit gas consumption (eu std.): %.1lf L/100km\n", 100 * gas / dist);
  return 0;
}
