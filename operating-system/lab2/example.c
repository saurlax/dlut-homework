#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5   // 缓冲区大小
#define NUM_PRODUCERS 2 // 生产者数量
#define NUM_CONSUMERS 2 // 消费者数量
#define NUM_ITEMS 20    // 每个生产者生产的物品数量

// 共享缓冲区
typedef struct
{
  int buffer[BUFFER_SIZE];  // 存储数据的数组
  int in;                   // 生产者放入数据的位置
  int out;                  // 消费者取出数据的位置
  int count;                // 当前缓冲区中的数据数量
  pthread_mutex_t mutex;    // 保护缓冲区的互斥锁
  pthread_cond_t not_full;  // 缓冲区不满的条件变量
  pthread_cond_t not_empty; // 缓冲区非空的条件变量
} Buffer;

Buffer shared_buffer;

// 初始化缓冲区
void buffer_init(Buffer *buffer)
{
  buffer->in = 0;
  buffer->out = 0;
  buffer->count = 0;
  pthread_mutex_init(&buffer->mutex, NULL);
  pthread_cond_init(&buffer->not_full, NULL);
  pthread_cond_init(&buffer->not_empty, NULL);
}

// 销毁缓冲区的同步对象
void buffer_destroy(Buffer *buffer)
{
  pthread_mutex_destroy(&buffer->mutex);
  pthread_cond_destroy(&buffer->not_full);
  pthread_cond_destroy(&buffer->not_empty);
}

// 生产者向缓冲区插入数据
void buffer_put(Buffer *buffer, int item)
{
  // 获取互斥锁
  pthread_mutex_lock(&buffer->mutex);

  // 如果缓冲区已满，等待不满条件
  while (buffer->count == BUFFER_SIZE)
  {
    printf("Buffer is FULL. Producer is waiting...\n");
    pthread_cond_wait(&buffer->not_full, &buffer->mutex);
  }

  // 向缓冲区插入数据
  buffer->buffer[buffer->in] = item;
  buffer->in = (buffer->in + 1) % BUFFER_SIZE; // 循环使用缓冲区
  buffer->count++;

  printf("Produced: %d (count: %d)\n", item, buffer->count);

  // 通知等待的消费者缓冲区现在非空
  pthread_cond_signal(&buffer->not_empty);

  // 释放互斥锁
  pthread_mutex_unlock(&buffer->mutex);
}

// 消费者从缓冲区获取数据
int buffer_get(Buffer *buffer)
{
  int item;

  // 获取互斥锁
  pthread_mutex_lock(&buffer->mutex);

  // 如果缓冲区为空，等待非空条件
  while (buffer->count == 0)
  {
    printf("Buffer is EMPTY. Consumer is waiting...\n");
    pthread_cond_wait(&buffer->not_empty, &buffer->mutex);
  }

  // 从缓冲区取出数据
  item = buffer->buffer[buffer->out];
  buffer->out = (buffer->out + 1) % BUFFER_SIZE; // 循环使用缓冲区
  buffer->count--;

  printf("Consumed: %d (count: %d)\n", item, buffer->count);

  // 通知等待的生产者缓冲区现在不满
  pthread_cond_signal(&buffer->not_full);

  // 释放互斥锁
  pthread_mutex_unlock(&buffer->mutex);

  return item;
}

// 生产者线程函数
void *producer(void *arg)
{
  int id = *(int *)arg;
  int i;

  for (i = 0; i < NUM_ITEMS; i++)
  {
    // 生产一个数据项
    int item = id * 100 + i;

    // 模拟生产过程
    usleep(rand() % 100000);

    // 放入缓冲区
    buffer_put(&shared_buffer, item);
  }

  printf("Producer %d finished.\n", id);
  free(arg); // 释放传入的参数内存
  return NULL;
}

// 消费者线程函数
void *consumer(void *arg)
{
  int id = *(int *)arg;
  int i;

  for (i = 0; i < (NUM_ITEMS * NUM_PRODUCERS) / NUM_CONSUMERS; i++)
  {
    // 从缓冲区获取数据
    int item = buffer_get(&shared_buffer);

    // 模拟消费过程
    usleep(rand() % 150000);

    // 消费数据
    printf("Consumer %d consumed item: %d\n", id, item);
  }

  printf("Consumer %d finished.\n", id);
  free(arg); // 释放传入的参数内存
  return NULL;
}

int main()
{
  pthread_t producers[NUM_PRODUCERS];
  pthread_t consumers[NUM_CONSUMERS];
  int i;

  // 初始化随机数种子
  srand(time(NULL));

  // 初始化缓冲区
  buffer_init(&shared_buffer);

  // 创建生产者线程
  for (i = 0; i < NUM_PRODUCERS; i++)
  {
    int *id = malloc(sizeof(int));
    *id = i;
    pthread_create(&producers[i], NULL, producer, id);
    printf("Created producer %d\n", i);
  }

  // 创建消费者线程
  for (i = 0; i < NUM_CONSUMERS; i++)
  {
    int *id = malloc(sizeof(int));
    *id = i;
    pthread_create(&consumers[i], NULL, consumer, id);
    printf("Created consumer %d\n", i);
  }

  // 等待所有生产者线程结束
  for (i = 0; i < NUM_PRODUCERS; i++)
  {
    pthread_join(producers[i], NULL);
  }

  // 等待所有消费者线程结束
  for (i = 0; i < NUM_CONSUMERS; i++)
  {
    pthread_join(consumers[i], NULL);
  }

  // 销毁缓冲区的同步对象
  buffer_destroy(&shared_buffer);

  printf("All done!\n");
  return 0;
}