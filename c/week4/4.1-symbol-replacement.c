/**  
 * Using if else statements, write a program that reads input up to #, replaces each period
 * with an exclamation mark, replaces each exclamation mark initially present with two
 * exclamation marks, and reports at the end the number of substitutions it has made.
 */
#include <stdio.h>

char str1[10000];
char str2[10000];
int len1 = 0, len2 = 0;
int count1 = 0, count2 = 0;

int main()
{
  printf("Please enter something:\n");

  char c;
  while ((c = getchar()) != '#')
  {
    str1[len1++] = c;
  }

  for (int i = 0; i < len1; i++)
  {
    if (str1[i] == '!')
    {
      str2[len2++] = '!';
      str2[len2++] = '!';
      count2++;
    }
    else
    {
      str2[len2++] = str1[i];
    }
  }

  for (int i = 0; i < len1; i++)
  {
    if (str1[i] == '.')
    {
      str1[i] = '!';
      count2++;
    }
  }

  printf("Converted '.' to '!' %d times\nConverted '!' to '!!' %d times\n", count1, count2);
  printf("Here's result:\n%s", str2);
  return 0;
}
