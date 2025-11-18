#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *worker(void *args) {
  printf("Hello world!\n");
  return NULL;
}

int main(int argc, char *argv[]) {
  pthread_t tid;

  if (pthread_create(&tid, NULL, worker, NULL)) {
    printf("Error creating thread\n");
    return 1;
  }

  printf("Main waits for thread to finish\n");

  pthread_join(tid, NULL);

  return 0;
}