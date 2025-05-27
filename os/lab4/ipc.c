#include "ipc.h"

// Shared memory for table
key_t table_key;
int* table_ptr;
int table_shmid;
int shm_flg;

// Semaphores
key_t supplier_key;
int supplier_sem;
key_t smoker_keys[SMOKER_NUM];
int smoker_sems[SMOKER_NUM];
int sem_flg;

int set_sem(key_t sem_key, int sem_val, int sem_flg) {
    int sem_id;
    Sem_uns sem_arg;
    if ((sem_id = semget(sem_key, 1, sem_flg)) < 0) {
        perror("semget error");
        exit(EXIT_FAILURE);
    }
    sem_arg.val = sem_val;
    if (semctl(sem_id, 0, SETVAL, sem_arg) < 0) {
        perror("semctl error");
        exit(EXIT_FAILURE);
    }
    return sem_id;
}

int down(int sem_id) {
    struct sembuf buf = {0, -1, SEM_UNDO};
    if (semop(sem_id, &buf, 1) < 0) {
        perror("semop down error");
        exit(EXIT_FAILURE);
    }
    return 0;
}

int up(int sem_id) {
    struct sembuf buf = {0, 1, SEM_UNDO};
    if (semop(sem_id, &buf, 1) < 0) {
        perror("semop up error");
        exit(EXIT_FAILURE);
    }
    return 0;
}