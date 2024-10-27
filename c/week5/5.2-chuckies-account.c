/**  
 * Chuckie Lucky won a million dollars (after taxes), which he places in an account that
 * earns 8% a year. On the last day of each year, Chuckie withdraws $100,000. Write a
 * program that finds out how many years it takes for Chuckie to empty his account.
 */
#include <stdio.h>

int main()
{
  int years = 0;
  double chuckie = 1000000;
  while (chuckie > 0)
  {
    years++;
    chuckie *= 1.08;
    chuckie -= 100000;
  }
  printf("It takes %d years for Chuckie to empty his account.\n", years);
  return 0;
}