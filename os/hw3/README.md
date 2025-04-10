用自己部署好的 Linux 环境，在命令行下编辑并编译多线程代码。

```c
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
```

编译并运行代码，观察输出结果。

```bash
Main waits for thread to finish
Hello world!
```
