#include "lib/ipc_manager.h"
#include "lib/read_settings.h"
#include "lib/utils.h"
#include <string.h>
#include <signal.h>
#include <errno.h>

#define PATHNAME "Makefile"
FILE* f_logs;

int rand_split();
static void sigHandler(int signum);
int rand_energy(int energy_raw);

int isActivated = 1;

int main(){
	f_logs = fopen("logs.txt", "w"); //opening file for write 
	srand(getpid());

	int msgq_id=get_msgq(PATHNAME);
	int sem_id=sem_get(PATHNAME);
	statistic *stats=attach_memory_block(PATHNAME);

	signal(SIGUSR1, sigHandler);
	signal(SIGTERM, sigHandler);
	signal(SIGINT,sigHandler);
	
	struct msgbuff message;

	int type;
	pid_t pid;
	int sono_scoria;
	int energia_assorbita;

	sem_reserve(sem_id,SEM_READY);	
	if(errno==EIDRM ||errno==EINVAL){
		exit(EXIT_SUCCESS);
	}
	wait_to_zero(sem_id,SEM_READY);
	if(errno==EIDRM ||errno==EINVAL){
		exit(EXIT_SUCCESS);
	}
	
	while(true){
		//ricezione messaggio
 		msgq_id=get_msgq(PATHNAME);
#ifdef DEBUG
	dprintf(1,YEL"[DINIBITORE] Coda di messaggi id %d\n",msgq_id);
#endif
		errno=0;
		message=receive_message(PATHNAME,1);
		if(isActivated){
			// info della risposta sarà (0/1)
			
			//funzione che determina se scinde o meno
			sono_scoria=rand_split();
			
			//funzione che determina quanta energia assorbire
			energia_assorbita=rand_energy(message.energiaLiberata);
			type=message.my_pid;
			send_message(PATHNAME,type,sono_scoria,energia_assorbita);
			
			sem_reserve(sem_id,SEM_STATS);
			if(errno==EIDRM ||errno==EINVAL){
				exit(EXIT_SUCCESS);
			}

			stats->q_energia_assorbita+=energia_assorbita;
			sem_release(sem_id,SEM_STATS,1);
			if(errno==EIDRM ||errno==EINVAL){
				exit(EXIT_SUCCESS);
			}
			if (sono_scoria){
#ifdef DEBUG
	dprintf(1,YEL"[DINIBITORE]Il numero di scorie è aumentato di 1\n");
#endif
				fprintf(f_logs, "Aumento di 1 il numero di scorie\n");
			}
#ifdef DEBUG
	dprintf(1,YEL"[DINIBITORE]L'inibitore ha assorbito %d energia", energia_assorbita);
#endif
			fprintf(f_logs, "Ho assorbito %d energia\n", energia_assorbita);
		}else{
			sono_scoria=0;
			//funzione che determina quanta energia assorbire
			energia_assorbita=0;
			type=message.my_pid;
			send_message(PATHNAME,type,sono_scoria,energia_assorbita);	
		}
	}
}

int rand_split(){
	int res = 0;
	int num = rand_generator(1,20); //Potrebbe cambiare
	if(num !=1){
		res = 1; // Split
	}
	return res;
}

int rand_energy(int energy_raw){
	return (energy_raw/100)*rand_generator(50, 99);
}

static void sigHandler(int signum){
	if(signum == SIGUSR1){ // Wait inibitore
		dprintf(1,"[INIBITORE]L'inibitore è ripartito\n");
	}else if(signum == SIGTERM){ // Terminazione inibitore
#ifdef DEBUG
	dprintf(1,YEL"[DINIBITORE]L'inibitore %d ha finito la sua esecuzione\n"RESET,getpid());
#endif
		fclose(f_logs);
		dprintf(1, BLU"I logs dell'inibitore sono in src/logs.txt\n");
		exit(EXIT_SUCCESS);
	}
	if(signum==SIGINT){
		if(isActivated==1){
			//! inibitore disattivato
			isActivated=0;
#ifdef DEBUG
	dprintf(1,YEL"[DINIBITORE]L'inibitore è stato disattivato\n");
#endif
			dprintf(1, "[Inibitore]Inibitore disattivato");
			fprintf(f_logs, "Inibitore disattivato\n");
		}else if(isActivated==0){
			//! inibitore attivato
			isActivated=1;
#ifdef DEBUG
	dprintf(1,YEL"[DINIBITORE]L'inibitore è stato attivato\n");
#endif
			dprintf(1, "[Inibitore]Inibitore attivato");
			fprintf(f_logs, "Inibitore attivato\n");
		}
	}
}