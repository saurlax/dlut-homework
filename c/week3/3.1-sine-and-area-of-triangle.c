/**  
 * 输入三角形的三个边长，计算最大角的正弦值及三角形面积。注意对三角形边长合理性的判断。
 */
#include <stdio.h>
#include <math.h>

int main()
{
  double a, b, c;
  double max_length, cos, area;
  printf("Please enter the three sides of the triangle:\n");
  scanf("%lf%lf%lf", &a, &b, &c);

  if (a + b <= c || b + c <= a || c + a <= b)
  {
    printf("Not a triangle");
    return 0;
  }

  max_length = a;
  cos = (b * b + c * c - a * a) / (2 * b * c);
  area = 0.5 * sqrt(1 - cos * cos) * b * c;
  if (b > max_length)
  {
    max_length = b;
    cos = (a * a + c * c - b * b) / (2 * a * c);
  }
  if (c > max_length)
  {
    max_length = c;
    cos = (a * a + b * b - c * c) / (2 * a * b);
  }
  printf("The sine of the maximum angle is: %lf\n", sqrt(1 - cos * cos));
  printf("The area of the triangle is: %lf", area);
  return 0;
}
