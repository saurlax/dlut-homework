/**  
 * Output the following asterisk pyramid
 *                    row#    Blanks  Asterisks
 *      *             0       5       1
 *     ***            1       4       3
 *    *****           2       3       5
 *   *******          3       2       7
 *  *********         4       1       9
 * ***********        5       0       11
 */
#include <stdio.h>

int main()
{
  int row, blanks, asterisks;
  for (row = 0; row < 6; row++)
  {
    for (blanks = 0; blanks < 5 - row; blanks++)
    {
      printf(" ");
    }
    for (asterisks = 0; asterisks < 2 * row + 1; asterisks++)
    {
      printf("*");
    }
    printf("\n");
  }
  return 0;
}