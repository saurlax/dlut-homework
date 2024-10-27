/**  
 * Daphne invests $100 at 10% simple interest. (That is, every year, the investment earns
 * an interest equal to 10% of the original investment.) Deirdre invests $100 at 5% interest
 * compounded annually. (That is, interest is 5% of the current balance, including previous
 * addition of interest.) Write a program that finds how many years it takes for the value
 * of Deirdre's investment to exceed the value of Daphne's investment. Also show the two
 * values at that time.
 */
#include <stdio.h>

int main()
{
  int years = 0;
  double daphne = 100;
  double deirdre = 100;
  while (deirdre <= daphne)
  {
    years++;
    daphne += 10;
    deirdre *= 1.05;
  }
  printf("After %d years, Deirdre's investment exceeds Daphne's investment.\n", years);
  printf("Daphne's investment: $%.2f\n", daphne);
  printf("Deirdre's investment: $%.2f\n", deirdre);
  return 0;
}