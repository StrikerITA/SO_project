#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lib/ipc_manager.h"
#include "lib/utils.h"
#define PATHNAME "Makefile"

int main(int argc, char * argv[]){
	srand(getpid());
	int numero=argc;
	int sem_id=sem_get(PATHNAME);
	char *values=argv[0];
    int num_atomi_nuovi=atoi(argv[1]);
	
	//dprintf(1,"[ATTIVATORE]L'attivatore %d e stato creato\n",getpid());
	statistic *stats=attach_memory_block(PATHNAME);
	
	sem_reserve(sem_id,SEM_READY);
	dprintf(1,"[ATTIVATORE]Ho prelevato 1 r\n");
	wait_to_zero(sem_id,SEM_READY);


	dprintf(1,"[ATTIVATORE]start\n");

	int numero_attivazioni=-1;
	while(true){
		
		wait_to_zero(sem_id,SEM_ACTIVATOR);
		if(errno==EIDRM ||errno==EINVAL){
			exit(EXIT_SUCCESS);
		}
		numero_attivazioni = num_atom_generator_v2(num_atomi_nuovi);
		sem_release(sem_id,SEM_ACTIVATOR, numero_attivazioni);
		if(errno==EIDRM ||errno==EINVAL){
			exit(EXIT_SUCCESS);
		}

		sem_reserve(sem_id,SEM_STATS);
		if(errno==EIDRM ||errno==EINVAL){
			exit(EXIT_SUCCESS);
		}

		stats->n_attivazioni_tot+=numero_attivazioni;
		stats->n_attivazioni_sec+=numero_attivazioni;
		
		sem_release(sem_id,SEM_STATS,1);
		if(errno==EIDRM ||errno==EINVAL){
			exit(EXIT_SUCCESS);
		}
	}
	
	dprintf(1,"[ATTIVATORE]L'alimentatore %d ha finito la sua esecuzione\n",getpid());
}
