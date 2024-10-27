/**  
 * The 1988 United States Federal Tax Schedule was the simplest in recent times. lt had
 * four categories, and each category had two rates. Here is a summary (dollar amounts are
 * taxable income):
 * 
 * Category                             Tax
 * -----------------------------------------------------------------------------------
 * Single                               15% of first $17850 plus 28% of excess
 * Head of Household                    15% of first $23,900 plus 28% of excess
 * Married, Joint                       15% of first $29,750 plus 28% of excess
 * Married, Separate                    15% of first $14875 plus 28% of excess
 * 
 * For example, a single wage earner with a taxable income of $20,000 owes 0.15 * $17850
 * + 0.28 * ($20,000-$17,850). Write a program that lets the user specify the tax category
 * and the taxable income and that then calculates the tax. Use a loop so that the user can
 * enter several tax cases.
 */
#include <stdio.h>

int main()
{
  int type, step;
  double income, tax;

  while (1)
  {
    printf(
        "0: Exit\n"
        "1: Single\n"
        "2: Head of Household\n"
        "3: Married, Joint\n"
        "4: Married, Sqparate\n"
        "Please choose your tax type: ");
    scanf("%d", &type);
    if (type < 1)
    {
      break;
    }
    if (type > 4)
    {
      printf("Invaild type!\n");
      continue;
    }
    printf("Please tell me your income: ");
    scanf("%lf", &income);
    switch (type)
    {
    case 1:
      step = 17850;
      break;
    case 2:
      step = 23900;
      break;
    case 3:
      step = 29750;
      break;
    case 4:
      step = 14875;
      break;
    }
    if (income > step)
    {
      tax = 0.15 * step + 0.28 * (income - step);
    }
    else
    {
      tax = 0.15 * income;
    }
    printf("Here's your tax: $%.2lf\n", tax);
  }
  return 0;
}
