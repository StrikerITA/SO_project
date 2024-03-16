#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lib/ipc_manager.h"
#include <errno.h>
#define PATHNAME "Makefile"

int main(int argc, char * argv[]){
	int sem_id=sem_get(PATHNAME);
	dprintf(1,"%s\n",argv[0]);
	dprintf(1,"%s\n",argv[1]);
	dprintf(1,"[ATOMO]L'atomo %d e stato creato\n",getpid());
	statistic *stats=attach_memory_block(PATHNAME);
	sem_reserve(sem_id,SEM_READY);
	wait_to_zero(sem_id,SEM_READY);
	dprintf(1,"[ATOMO]start\n");

	for(int i=0;i< 30;i++){
		sleep(1);
		sem_reserve(sem_id,SEM_STAT);
		//dprintf(1,"[ATOMO]errno:%d\n",errno);
		if(errno==EIDRM ||errno==EINVAL){
			//dprintf(1,"[ATOMO]L'atomo errore:%d\n",errno);
			//sleep(4);
			exit(EXIT_SUCCESS);
		}
		stats->n_attivazioni_tot++;
		stats->n_attivazioni_sec++;
		sem_release(sem_id,SEM_STAT,1);
	}
	detach_memory_block(stats);
	
	dprintf(1,"[ATOMO]L'atomo %d a finito la sua esecuzione\n",getpid());

}
