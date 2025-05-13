#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

int sum = 0;
sem_t sem;

void *thread(void *)
{
  int i;
  for (i = 0; i < 1000000; i++)
  {
    sem_wait(&sem);
    sum++;
    sem_post(&sem);
  }
}

int main()
{
  pthread_t t1, t2;
  sem_init(&sem, 0, 1);
  pthread_create(&t1, NULL, thread, NULL);
  pthread_create(&t2, NULL, thread, NULL);
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  printf("sum = %d\n", sum);
  sem_destroy(&sem);
  return 0;
}