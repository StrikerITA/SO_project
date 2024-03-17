#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lib/utils.h"
#include "lib/ipc_manager.h"
#include <errno.h>
#define PATHNAME "Makefile"

int main(int argc, char * argv[]){
	int sem_id=sem_get(PATHNAME);
	//TODO: mettere if per argc
	
	int numero=argc;
	int num_atomic=atoi(argv[1]);
	int min_numero_atomico=atoi(argv[2]);
	pid_t master_pid=atoi(argv[3]);
	int first_atom=atoi(argv[4]);
	//dprintf(1,"%d",first_atom);
	/*dprintf(1,"%s\n",argv[0]);
	dprintf(1,"%s\n",argv[1]);
	dprintf(1,"[ATOMO]L'atomo %d e stato creato\n",getpid());*/
	
	statistic *stats=attach_memory_block(PATHNAME);
	
	//Variabili necessare per scissione
	pid_t atomo;
	char process_name[20];
	char param1[20];
	char param2[20];
	char param3[20];
	char param4[20];
	char param5[20];
	char *args[7];

	srand(getpid());
	strcpy(process_name,"atom");
	args[0]=process_name;

	//Finite preparazioni
	if(first_atom==1){
	sem_reserve(sem_id,SEM_READY);
	//dprintf(1,"[ATOMO]Ho prelevato 1 r\n");
		if(errno==EIDRM ||errno==EINVAL){
			exit(EXIT_SUCCESS);
		}
		wait_to_zero(sem_id,SEM_READY);
		if(errno==EIDRM ||errno==EINVAL){
			exit(EXIT_SUCCESS);
		}
	}
	
	int num_atomic_figlio;

	while(true){
		//sleep(1);
		//Chiedo scissione
		sem_reserve(sem_id, SEM_ACTIVATOR);
		if(errno==EIDRM ||errno==EINVAL){
			exit(EXIT_SUCCESS);
		}
		//...Verificare se e scoria
		if(num_atomic < min_numero_atomico){
			sem_reserve(sem_id,SEM_STATS);
			if(errno==EIDRM ||errno==EINVAL){
				exit(EXIT_SUCCESS);
			}	
			stats->n_scorie_tot++;
			stats->n_scorie_sec++;
			sem_release(sem_id,SEM_STATS,1);
			if(errno==EIDRM ||errno==EINVAL){
				exit(EXIT_SUCCESS);
			}

			exit(EXIT_SUCCESS);
		}
		//Calcolo num atomico e energia liberata
		num_atomic_figlio = rand_num_atom(1, num_atomic);
		num_atomic=num_atomic-num_atomic_figlio;
		sprintf(param1,"%d",num_atomic_figlio);
		sprintf(param2,"%d",min_numero_atomico);
		sprintf(param3,"%d",master_pid);
		sprintf(param4,"%d",0);
		args[1]=param1;
		args[2]=param2;
		args[3]=param3;
		args[4]=param4;
		args[5]=NULL;

		//creazione nuovo atomo
		atomo=create_process(process_name, args, master_pid);

		sem_reserve(sem_id,SEM_STATS);
		if(errno==EIDRM ||errno==EINVAL){
			exit(EXIT_SUCCESS);
		}
		//carico i dati dentro le stats
		stats->n_scissioni_tot++;
		stats->n_scissioni_sec++;
		sem_release(sem_id,SEM_STATS,1);
		if(errno==EIDRM ||errno==EINVAL){
			exit(EXIT_SUCCESS);
		}
	}

	/*for(int i=0;i< 30;i++){
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
	}*/

	//TODO: fare gestione scorie

	detach_memory_block(stats);
	
	dprintf(1,"[ATOMO]L'atomo %d a finito la sua esecuzione\n",getpid());

}