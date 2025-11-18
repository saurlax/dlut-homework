#include "ipc.h"

const char *material_names[MATERIAL_NUM] = {"Tobacco", "Paper", "Glue"};

int main()
{
  srand(time(NULL) ^ getpid());
  shm_flg = IPC_CREAT | 0644;
  sem_flg = IPC_CREAT | 0644;

  // Create shared memory for table
  table_key = 1001;
  table_shmid = shmget(table_key, MATERIAL_NUM * sizeof(int), shm_flg);
  if (table_shmid < 0)
  {
    perror("shmget error");
    exit(EXIT_FAILURE);
  }
  table_ptr = (int *)shmat(table_shmid, 0, 0);

  // Create semaphores
  supplier_key = 2001;
  supplier_sem = set_sem(supplier_key, 1, sem_flg);
  for (int i = 0; i < SMOKER_NUM; ++i)
  {
    smoker_keys[i] = 3001 + i;
    smoker_sems[i] = set_sem(smoker_keys[i], 0, sem_flg);
  }

  while (1)
  {
    // Wait for DingZhenSmoker to finish smoking
    down(supplier_sem);

    // Change from random to round-robin selection of missing material
    static int lack = -1;
    if (lack == -1) {
      // Initialize with a random value on first run
      lack = rand() % MATERIAL_NUM;
    }
    
    for (int i = 0; i < MATERIAL_NUM; ++i)
    {
      table_ptr[i] = (i == lack) ? 0 : 1;
    }

    printf("SnowLeopardSupplier puts: %s and %s on the table\n",
           material_names[(lack + 1) % MATERIAL_NUM],
           material_names[(lack + 2) % MATERIAL_NUM]);

    // Wake up the corresponding DingZhenSmoker
    up(smoker_sems[lack]);
    
    // Move to next material in round-robin fashion
    lack = (lack + 1) % MATERIAL_NUM;

    sleep(1);
  }
  return 0;
}