#include "ipc_manager.h"

int sem_create(char *pathname){
	key_t key;
	key=ftok(pathname,SEMAPHORE);
	return semget(key,NUMBER_SEMAPHORES,IPC_CREAT| 0666| IPC_EXCL);
}
int sem_destroy(char *pathname){
	int id=sem_get(pathname);
	return semctl(id,NUMBER_SEMAPHORES,IPC_RMID);
}
int sem_get(char *pathname){
	key_t key=ftok(pathname,SEMAPHORE);
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
	int ret=semop(sem_id, &sops, 1);
	return ret;
}

int sem_release(int sem_id, sem_types sem_num, int num_resources) {
	struct sembuf sops;
	sops.sem_num = sem_num;
	sops.sem_op = num_resources;
	sops.sem_flg = 0;
	int ret=semop(sem_id, &sops, 1);
	return ret;
}
int wait_to_zero(int sem_id,sem_types sem_num){
	struct sembuf sops;
	sops.sem_num=sem_num;
	sops.sem_op=0;
	sops.sem_flg=0;
	int ret= semop(sem_id,&sops,1);
	return ret;
}

//-------------------------------------------------
//-------------SHARED-MEMORY-----------

int get_shmem(char *filename){
	key_t key=ftok(filename,SHARED_MEMORY);
	return shmget(key,sizeof(statistic),0);
}

int create_shmem(char *filename){
    key_t key;
	int size=sizeof(statistic);
    
    key=ftok(filename,SHARED_MEMORY);
    return shmget(key, size, 0644| IPC_CREAT); 
}

statistic * attach_memory_block(char *filename){
    int shared_block_id=get_shmem(filename);
    statistic *result;
    result =shmat(shared_block_id,NULL,0);
    return result;
}

bool detach_memory_block(statistic *block){
    return(shmdt(block)!=IPC_RESULT_ERROR);
}

bool destroy_memory_block(char *filename){
    int shared_block_id=get_shmem(filename);
    if(shared_block_id==IPC_RESULT_ERROR){
        return NULL;
    }
    return(shmctl(shared_block_id,IPC_RMID,NULL)!= IPC_RESULT_ERROR);
}

//-----------------------------------------------
//---------------MESSAGE-QUEUE---------------


int send_message(char *filename,int type,int sonoScoria,int energiaLiberata){
	struct msgbuff message;
	message.msg_type=type;
	message.my_pid=getpid();
	message.sonoScoria=sonoScoria;
	message.energiaLiberata=energiaLiberata;
	int msgq_id=get_msgq(filename);
	
	int ris=msgsnd(msgq_id,&message,(2*sizeof(int)+sizeof(pid_t)),0);
	return ris;	
}

struct msgbuff receive_message(char *filename,long msg_type){
	struct msgbuff message;
	int msgq_id=get_msgq(filename);
	msgrcv(msgq_id,&message,(2*sizeof(int)+sizeof(pid_t)),msg_type,0);
	return message;
}

int create_msgq(char *filename){
    key_t key;
    key=ftok(filename,MESSAGE_QUEUE);
    return msgget(key, 0644| IPC_CREAT|IPC_EXCL); 
}

int get_msgq(char *filename){
	key_t key;
	key=ftok(filename,MESSAGE_QUEUE);
	return msgget(key,0);
}

int destroy_msgq(char *filename){
	int msgq_id=get_msgq(filename);
	return msgctl(msgq_id,IPC_RMID,NULL);
}