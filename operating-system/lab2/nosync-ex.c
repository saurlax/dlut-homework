#include <stdio.h>
#include <pthread.h>

int sum = 0;

void *thread(void *)
{
  int i;
  for (i = 0; i < 1000000; i++)
  {
    sum++;
  }
}

int main()
{
  pthread_t t1, t2;
  pthread_create(&t1, NULL, thread, NULL);
  pthread_create(&t2, NULL, thread, NULL);
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  printf("sum = %d\n", sum);
  return 0;
}