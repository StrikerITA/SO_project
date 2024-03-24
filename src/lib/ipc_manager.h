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
struct msgbuff{
	long int msg_type;
	pid_t my_pid;
	/*
		Se msgType=1 allora info sara energia liberata totale //assorbe 
		Se msgType=1 allora info sara -1; //decidere se ce scissione o meno
		Se msgType >2(pid processo richiedente) allora info sara 0/1 oppure
		sara energia liberata inibita
	*/
	int info;	
};

int send_message(char *filename,int type,int info);
struct msgbuff receive_message(char *filename,long msg_type);
int create_msgq(char *filename);
int get_msgq(char *filename);
int destroy_msgq(char *filename);