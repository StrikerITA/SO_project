#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lib/utils.h"
#include "lib/ipc_manager.h"
#include <errno.h>
#define PATHNAME "Makefile"

int energy(int n1, int n2);
int max(int n1, int n2);
int isActivated=0;
static void sigHandler(int signum);

int main(int argc, char * argv[]){
	int sem_id=sem_get(PATHNAME);
	//TODO: mettere if per argc
	
	signal(SIGINT,NULL);
	int numero=argc;
	int num_atomic=atoi(argv[1]);
	int min_numero_atomico=atoi(argv[2]);
	pid_t master_pid=atoi(argv[3]);
	int first_atom=atoi(argv[4]);
	int energy_explode_threshold = atoi(argv[5]);
	
	statistic *stats=attach_memory_block(PATHNAME);
	if(get_msgq(PATHNAME)==-1){
		isActivated=0;
	}else{
		isActivated=1;
	}
	//Variabili necessare per scissione
	pid_t atomo;
	char process_name[20];
	char param1[20];
	char param2[20];
	char param3[20];
	char param4[20];
	char param5[20];
	char param6[20];
	char *args[8];

	srand(getpid());
	strcpy(process_name,"atom.out");
	args[0]=process_name;

	//Finite preparazioni
	if(first_atom==1){
	sem_reserve(sem_id,SEM_READY);
	//dprintf(1,"[ATOMO]Ho prelevato 1 r\n");
		if(errno==EIDRM ||errno==EINVAL){
			exit(EXIT_SUCCESS);
		}
#ifdef DEBUG 
	dprintf(1,"[DATOM] Aspetto avvio simulazione\n");
#endif
		wait_to_zero(sem_id,SEM_READY);
		if(errno==EIDRM ||errno==EINVAL){
			exit(EXIT_SUCCESS);
		}
	}
	
	int num_atomic_figlio;
	int energia_liberata;
	int new_num_atom=-1;
	struct msgbuff message;

	while(true){
		//Chiedo scissione
		sem_reserve(sem_id, SEM_ACTIVATOR);

		if(errno==EIDRM ||errno==EINVAL){
			exit(EXIT_SUCCESS);
		}else if(errno==EINTR){
			continue;
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
#ifdef DEBUG
#endif
		//Calcolo num atomico
		num_atomic_figlio = rand_generator(1, num_atomic-1);
		new_num_atom=num_atomic-num_atomic_figlio;
#ifdef DEBUG
	//dprintf(1,"[ATOMO-DEBUG]Num_atomic: %d,New_num_atom:%d, Num_atomic_figlio: %d\n",num_atomic,new_num_atom,num_atomic_figlio);

	///dprintf(1,"[ATOMO-DEBUG]Num: %d\n",energia_liberata);
#endif
		num_atomic=new_num_atom;
		//Calcolo energia liberata energy(n1,n2) = n1 * n2 - max(n1 | n2)
		energia_liberata = energy(num_atomic, num_atomic_figlio);
		
#ifdef DEBUG
	dprintf(1,YEL"[ATOMO-DEBUG]Energia liberata: %d\n"RESET,energia_liberata);
#endif
		
		sprintf(param1,"%d",num_atomic_figlio);
		sprintf(param2,"%d",min_numero_atomico);
		sprintf(param3,"%d",master_pid);
		sprintf(param4,"%d",0);
		sprintf(param5,"%d",energy_explode_threshold);
		args[1]=param1;
		args[2]=param2;
		args[3]=param3;
		args[4]=param4;
		args[5]=param5;
		args[6]=NULL;

		//creazione nuovo atomo
		atomo=create_process(process_name, args, master_pid);
		
#ifdef DEBUG 
	dprintf(1,YEL"[DATOM] Ho fatto la scissione\n"RESET);
#endif
		//dprintf(1,"[ATOMO]%d \n",isActivated);
		if(isActivated){
			send_message(PATHNAME,1,-1,energia_liberata);
			message= receive_message(PATHNAME,getpid());
			energia_liberata=energia_liberata-message.energiaLiberata;
			if(message.sonoScoria==1){
				sem_reserve(sem_id,SEM_STATS);
				if(errno==EIDRM ||errno==EINVAL){
					exit(EXIT_SUCCESS);
				}
				//carico dati energia liberata
				stats->q_energia_prodotta_sec+=energia_liberata;
				stats->q_energia_prodotta_tot+=energia_liberata;
				stats->n_scorie_sec++;
				stats->n_scorie_tot++;
				sem_release(sem_id,SEM_STATS,1);
				if(errno==EIDRM ||errno==EINVAL){
					exit(EXIT_SUCCESS);
				}
				exit(EXIT_SUCCESS);
			}

		}

		sem_reserve(sem_id,SEM_STATS);
		if(errno==EIDRM ||errno==EINVAL){
			exit(EXIT_SUCCESS);
		}

		//carico dati energia liberata
		stats->q_energia_prodotta_sec+=energia_liberata;
		stats->q_energia_prodotta_tot+=energia_liberata;
		
		if (stats->q_energia_prodotta_tot > energy_explode_threshold){
	#ifdef DEBUG
			dprintf(1,RED"[DATOM]Genero errore explode\n"RESET);
	#endif
			kill(master_pid,SIGUSR1);
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

	detach_memory_block(stats);
	
	//dprintf(1,"[ATOMO]L'atomo %d ha finito la sua esecuzione\n",getpid());

}

static void sigHandler(int signum){
	if(signum==SIGINT){
		if(isActivated==1){
			isActivated=0;
			dprintf(1,"[ATOMO]Coda di messaggi Disattivata\n");
		}else if(isActivated==0){
			isActivated=1;
			dprintf(1,"[ATOMO]Coda di messaggi Attivata\n");
		}
	}
}


int energy(int n1, int n2){	
	int n=n1*n2;
	return ((n1 * n2) - max(n1, n2));
}

int max(int n1, int n2){
	if (n1 > n2){
		return n1;
	}else{
		return n2;
	}
}