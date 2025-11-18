#ifndef SMOKERS_IPC_H
#define SMOKERS_IPC_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <time.h>

#define MATERIAL_NUM 3  // Tobacco, Paper, Glue
#define SUPPLIER_NUM 2
#define SMOKER_NUM 3

typedef union semuns {
    int val;
} Sem_uns;

int set_sem(key_t sem_key, int sem_val, int sem_flg);
int down(int sem_id);
int up(int sem_id);

// Shared memory for table (3 ints: tobacco, paper, glue)
extern key_t table_key;
extern int* table_ptr;
extern int table_shmid;
extern int shm_flg;

// Semaphores
extern key_t supplier_key;
extern int supplier_sem;
extern key_t smoker_keys[SMOKER_NUM];
extern int smoker_sems[SMOKER_NUM];
extern int sem_flg;

#endif