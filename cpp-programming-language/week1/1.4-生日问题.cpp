/**
 * 假设一个班级有50名同学，每个同学都报出自己的生日，每个同学的生日都不相重的概率只有0.0296，
 * 如果有100个同学，不相重的概率为3.0725*10^-7。相重复的概率如此之大与我们主观想象不同。
 * 写一个程序，输入同学的人数n，利用数学方法计算出其生日不重复的概率。然后再用仿真的方法，
 * 利用随机数产生每个同学的生日，计算出生日不重复的概率并与前面的结果进行比较。
 *
 * 随机数方法中，学生的生日可以用随机函数rand()产生，
 * 调用rand()前首先调用srand(long int*)函数设置种子。
 * 以上函数需要包含头文件cstdlib。
 */

#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

double probability(int n) {
  double p = 1;
  for (int i = 1; i < n; i++) {
    p *= (365 - i) / 365.0;
  }
  return p;
}

int *birthdays;

bool once(int n) {
  for (int i = 0; i < n; i++) {
    birthdays[i] = rand() % 365;
  }
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      if (birthdays[i] == birthdays[j]) {
        return false;
      }
    }
  }
  return true;
}

double simulation(int n) {
  const double times = 10e5;
  int count = 0;
  for (int i = 0; i < times; i++) count += once(n);
  return count / times;
}

int main() {
  int n;
  cin >> n;
  birthdays = new int[n];
  cout << "probability: " << probability(n) << endl;
  cout << "simulation: " << simulation(n) << endl;
  return 0;
}