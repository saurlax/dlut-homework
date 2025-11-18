/**  
 * Write a program to output the average of 2 integers.
 */
#include <stdio.h>

int main()
{
  int a, b;
  scanf("%d%d", &a, &b);
  printf("%.2lf", (a + b) / 2.0);
  return 0;
}
