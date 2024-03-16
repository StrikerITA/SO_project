#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"

#include <errno.h>
#include <unistd.h>

#include <stdbool.h>

#include <sys/shm.h>
#include <sys/msg.h>

#define IPC_KEY_SIMULATION 34197
#define NUMBER_SEMAPHORES 3






//-------------------
//-----Semafori------
//Commentare cosa fanno
//Manca gestione degli errori con errno
int sem_create(char *pathname);
int sem_destroy(char *pathname);
int sem_get(char *pathname);
int sem_set_val(int sem_id, sem_types sem_num, int sem_val);
int sem_reserve(int sem_id, sem_types sem_num);
int sem_release(int sem_id, sem_types sem_num, int num_resources);
int wait_to_zero(int sem_id,sem_types sem_num);

//--------------------
//Memoria condivisa

#define TEST_SEM_ERROR if (errno) {             \
                        fprintf(stderr,		\
				       "%s:%d: PID=%5d: Error %d (%s)\n", \
				       __FILE__,			\
				       __LINE__,			\
				       getpid(),			\
				       errno,				\
				       strerror(errno));    \
                       }

int create_shmem(char *filename);
int get_shmem(char *filename);
statistic * attach_memory_block(char *filename);
bool detach_memory_block(statistic *block);
bool destroy_memory_block(char *filename);



//Message queue