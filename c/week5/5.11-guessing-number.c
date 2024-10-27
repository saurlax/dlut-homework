/**  
 * 编程实现猜数字游戏。由程序自动生成1-10之间的一个随机数。游戏者通过键盘输入猜测的数字，
 * 如果猜对了则游戏结束，否则输出提示信息，显示所猜的数是太大还是太小。游戏结束后输出猜的
 * 次数。游戏规定猜的次数不能超过6次，若多于6次仍未猜中则自动游戏结束，并输出“输局”的信息。
 * 在每次玩家继续输入新的猜测数时，可以选择是否退出游戏。
 * 注：C语言在标准库中提供了产生随机数的函数rand()，使用时要include <stdlib.h>。
 * 该函数一般使用形式为rand()%a+b  变量a表示随机数产生的范围（0到a），变量b表示该范围的
 * 起始值，如rand()%100+1 产生1-100的随机数。调用该函数每次会产生一个伪随机数，即每次运行
 * 产生的随机数是相同的。可以通过提供不同的“随机种子”来实现每次产生不同的随机数。例如：
 *
 * #include <stdlib.h>
 * #include <time.h>
 * srand(time(0));  // 将系统当前时间作为随机种子
 * int magic = rand()%100+1; // 利用rand() 产生1-100之间的随机数，每次运行都不同
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
  srand(time(0));
  int magic = rand() % 10 + 1;
  puts("Guess a number between 1 and 10");
  for (int i = 0; i < 6; i++)
  {
    int guess;
    printf("Your guess (0 to exit): ");
    scanf("%d", &guess);

    if (guess == 0)
    {
      puts("Bye");
      return 0;
    }
    else if (guess == magic)
    {
      printf("Congratulations! You guessed out in %d attempts\n", i + 1);
      return 0;
    }
    else if (guess > magic)
    {
      puts("Too big");
    }
    else
    {
      puts("Too small");
    }
  }
  printf("You lose! The number is %d\n", magic);
  return 0;
}