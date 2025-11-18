#include "ipc.h"

const char *material_names[MATERIAL_NUM] = {"Tobacco", "Paper", "Glue"};

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("usage: %s [0|1|2]\n", argv[0]);
    printf("0 for Tobacco, 1 for Paper, 2 for Glue\n");
    exit(1);
  }
  int my_material = atoi(argv[1]); // 0:Tobacco, 1:Paper, 2:Glue

  shm_flg = IPC_CREAT | 0644;
  sem_flg = IPC_CREAT | 0644;

  // Attach shared memory
  table_key = 1001;
  table_shmid = shmget(table_key, MATERIAL_NUM * sizeof(int), shm_flg);
  if (table_shmid < 0)
  {
    perror("shmget error");
    exit(EXIT_FAILURE);
  }
  table_ptr = (int *)shmat(table_shmid, 0, 0);

  // Get semaphores
  supplier_key = 2001;
  supplier_sem = set_sem(supplier_key, 0, sem_flg);
  for (int i = 0; i < SMOKER_NUM; ++i)
  {
    smoker_keys[i] = 3001 + i;
    smoker_sems[i] = set_sem(smoker_keys[i], 0, sem_flg);
  }

  while (1)
  {
    // Wait for SnowLeopardSupplier to provide materials
    down(smoker_sems[my_material]);
    printf("DingZhenSmoker %d has %s, takes %s and %s, makes and smokes a cigarette...\n",
           my_material,
           material_names[my_material],
           material_names[(my_material + 1) % MATERIAL_NUM],
           material_names[(my_material + 2) % MATERIAL_NUM]);

    sleep(2);

    // Notify SnowLeopardSupplier to provide next materials
    up(supplier_sem);
  }
  return 0;
}