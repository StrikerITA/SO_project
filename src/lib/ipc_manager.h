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
#define IPC_RESULT_ERROR (-1) 
#define NUMBER_SEMAPHORES 3

//-------------------
//-----Semafori------
//Commentare cosa fanno
/*
	Crea un nuovo semaforo
	
	@param pathname > Serve per creare ed identificare un oggetto IPCS
	@return ID del semaforo
*/ 
int sem_create(char *pathname);

/*
	Distrugge il semaforo desiderato
	
	@param pathname > Serve per identificare un oggetto IPCS
	@return 0 -> success or -1 + errno -> failure
*/ 
int sem_destroy(char *pathname);

/*
	Restituisce l'ID del semaforo desiderato
	
	@param pathname > Serve per identificare un oggetto IPCS
	@return sem_id -> ID del semaforo desiderato
*/ 
int sem_get(char *pathname);

/*
	Imposta un valore iniziale al semaforo desiderato
	
	@param sem_id > id semaforo
	@param sem_num > tipologia di semaforo
	@param sem_val > numero di risorse da impostare allo start del semaforo
	@return 0 -> success or -1 + errno -> failure
*/
int sem_set_val(int sem_id, sem_types sem_num, int sem_val);

/*
	Toglie una risorsa al semaforo desiderato
	
	@param sem_id > id semaforo
	@param sem_num > tipologia di semaforo
	@return 0 -> success or -1 + errno -> failure
*/
int sem_reserve(int sem_id, sem_types sem_num);

/*
	Rilascia x risorse al semaforo desiderato
	
	@param sem_id > id semaforo
	@param sem_num > tipologia di semaforo
	@param num_resources > numero di risorse rilasciate
	@return 0 -> success or -1 + errno -> failure
*/ 
int sem_release(int sem_id, sem_types sem_num, int num_resources); 

/*
	Aspetta che il semaforo arrivi a 0
	
	@param sem_id > id semaforo
	@param sem_num > tipologia di semaforo
	@return 0 -> success or -1 + errno -> failure
*/
int wait_to_zero(int sem_id,sem_types sem_num);

int create_shmem(char *filename);
int get_shmem(char *filename);
statistic * attach_memory_block(char *filename);
bool detach_memory_block(statistic *block);
bool destroy_memory_block(char *filename);

/*
	@brief * Message queue *

	@param msgType = 1 allora info sara energia liberata totale assorbe 
	@param msgType = 1 allora info sara -1; //decidere se ce scissione o meno
	@param msgType > 2 (pid processo richiedente) allora info sara 0/1 oppure sarà energia liberata inibita
*/
struct msgbuff{
	long int msg_type;
	pid_t my_pid;
	int sonoScoria;	
	int energiaLiberata;
};

int send_message(char *filename,int type,int sonoScoria,int energiaLiberata);
struct msgbuff receive_message(char *filename,long msg_type);
int create_msgq(char *filename);
int get_msgq(char *filename);
int destroy_msgq(char *filename);