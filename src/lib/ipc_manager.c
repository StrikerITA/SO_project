#include "ipc_manager.h"

//
int sem_create(char *pathname){
	key_t key;
	key=ftok(pathname,SEMAPHORE);
	printf("La mia chiave e: %d",key);
	return semget(key,NUMBER_SEMAPHORES,IPC_CREAT| 0666| IPC_EXCL);
}
int sem_destroy(char *pathname){
	key_t key;
	key=ftok(pathname,SEMAPHORE);
	printf("La mia chiave e: %d\n",key);
	int id=sem_get(key);
	printf("Il mio ID e: %d\n",id);
	return semctl(id,NUMBER_SEMAPHORES,IPC_RMID);
}
int sem_get(key_t key){
	return semget(key,NUMBER_SEMAPHORES,0);
}

int sem_set_val(int sem_id, sem_types sem_num, int sem_val) {
	return semctl(sem_id, sem_num, SETVAL, sem_val);
}

int sem_reserve(int sem_id, sem_types sem_num) {
	struct sembuf sops;
	sops.sem_num = sem_num;
	sops.sem_op = -1;
	sops.sem_flg = 0;
	return semop(sem_id, &sops, 1);
}

int sem_release(int sem_id, sem_types sem_num, int num_resources) {
	struct sembuf sops;
	sops.sem_num = sem_num;
	sops.sem_op = num_resources;
	sops.sem_flg = 0;
	return semop(sem_id, &sops, 1);
}
int wait_to_zero(int sem_id,sem_types sem_num){
	struct sembuf sops;
	sops.sem_num=sem_num;
	sops.sem_op=0;
	sops.sem_flg=0;
	return semop(sem_id,&sops,1);
}

//-------------------------------------------------
//-------------SHARED-MEMORY-----------
#define IPC_RESULT_ERROR (-1) //
#define BLOCK_SIZE 4096
#define FILENAME "writeshmem.c"
int get_shmem(char *filename){
	key_t key=ftok(filename,SHARED_MEMORY);
	return shmget(key,sizeof(statistic),0);
}

int create_shmem(char *filename){
    key_t key;
	int size=sizeof(statistic);
    //Request a key
    //The key is linked to a filename, so that other programs can access it
    key=ftok(filename,SHARED_MEMORY);//ftok returns a key based on path and id 
    /*if(key==IPC_RESULT_ERROR){
        return IPC_RESULT_ERROR;
    }*/

    //get shared block --- create it if it doesn't exist
    //return ID of the shared block
    //0644| IPC_CREAT permissions
    return shmget(key, size, 0644| IPC_CREAT); 
}

statistic * attach_memory_block(char *filename){

    int shared_block_id=get_shmem(filename);
    statistic *result;
    result =shmat(shared_block_id,NULL,0);
	//TODO:Gestione errori
    return result;
}

bool detach_memory_block(statistic *block){
    return(shmdt(block)!=IPC_RESULT_ERROR);
	//TODO:Gestione errori
}

bool destroy_memory_block(char *filename){
    int shared_block_id=get_shmem(filename);
    if(shared_block_id==IPC_RESULT_ERROR){
        return NULL;
    }

    return(shmctl(shared_block_id,IPC_RMID,NULL)!= IPC_RESULT_ERROR);
}

//-----------------------------------------------
//---------------Message-queue---------------

int create_msgq(char *filename){
    key_t key;
    key=ftok(filename,MESSAGE_QUEUE);
    return msgget(key, 0644| IPC_CREAT); 
}
int get_msgq(char *filename){
	key_t key;
	key=ftok(filename,MESSAGE_QUEUE);
	return msgget(key,0);
}
int destroy_msgq(char *filename){
	int msgq_id=get_msgq(filename);
	//return msgctl(msgq_id,IPC_RMID);
}