#include <stdio.h>
#include <pthread.h>

int sum = 0;
pthread_mutex_t mutex;

void *thread(void *)
{
  int i;
  for (i = 0; i < 1000000; i++)
  {
    pthread_mutex_lock(&mutex);
    sum++;
    pthread_mutex_unlock(&mutex);
  }
}

int main()
{
  pthread_t t1, t2;
  pthread_mutex_init(&mutex, NULL);
  pthread_create(&t1, NULL, thread, NULL);
  pthread_create(&t2, NULL, thread, NULL);
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  printf("sum = %d\n", sum);
  pthread_mutex_destroy(&mutex);
  return 0;
}