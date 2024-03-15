#include "ipc_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#define PATHNAME "Makefile"


int main(){
	int sem_id,shmem_id;
	sem_id=sem_create(PATHNAME);
	printf("Semaforo Creato con id: %d\n",sem_id);
	shmem_id=create_shmem(PATHNAME);
	printf("Memoria Condivisa con id: %d\n",shmem_id);
	
	
	sleep(10);
	sem_destroy(PATHNAME);
	printf("Semaforo Rimosso\n");
	destroy_memory_block(PATHNAME);
	printf("MEmoria Condivisa Rimossa\n");

}