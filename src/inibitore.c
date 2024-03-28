#include "lib/ipc_manager.h"
#include "lib/read_settings.h"
#include "lib/utils.h"
#include <string.h>
#include <signal.h>
#include <errno.h>

#define PATHNAME "Makefile"

int rand_split();
static void sigHandler(int signum);
int rand_energy(int energy_raw);

int isActivated;

int main(int argc, char *argv[]){
	srand(getpid());
	// filtrare errori argv
	int num_param=argc;
	char *mia_variabile=argv[0];
	if(get_msgq(PATHNAME)==-1){
		isActivated=0;
	}
	else{
		isActivated=1;
	}
	int msgq_id=get_msgq(PATHNAME);
	dprintf(1,"[INIBITORE]accesso alla coda di messaggi con id =%d\n",msgq_id);
	int sem_id=sem_get(PATHNAME);
	statistic *stats=attach_memory_block(PATHNAME);

	signal(SIGUSR1, sigHandler);
	signal(SIGTERM, sigHandler);
	signal(SIGINT,sigHandler);
	
	struct msgbuff message;

	int type;
	pid_t pid;
	int sono_scoria;
	int energia_liberata;

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
		//dprintf(1,"coda\n");

		//dprintf(1, "num msg: %d", check_msgq(PATHNAME));
		//!! si blocca in ricezione del messaggio

		//sleep(1);
 		msgq_id=get_msgq(PATHNAME);
 		//dprintf(1,"[INIBITORE] Coda di messaggi id %d\n",msgq_id);
		// se ci sono messaggi nella coda, accede
		//if (check_msgq(PATHNAME) > 0){
			errno=0;
		message=receive_message(PATHNAME,1);
		if( errno==EINTR){
			//dprintf(1," my errno %d\n",errno);
			continue;
		}
		if(isActivated){
			// info della risposta sara (0/1)
			//DONE:funzione che determina se scinde o meno
			sono_scoria=rand_split();
			//DONE:funzione che determina quanta energia assorbire
			energia_liberata=rand_energy(message.energiaLiberata);
			type=message.my_pid;
			//dprintf(1,"%d %d \n",sono_scoria,energia_liberata);
			send_message(PATHNAME,type,sono_scoria,energia_liberata);
			
			sem_reserve(sem_id,SEM_STATS);
			if(errno==EIDRM ||errno==EINVAL){
				exit(EXIT_SUCCESS);
			}
			stats->q_energia_assorbita+=energia_liberata;		
			sem_release(sem_id,SEM_STATS,1);
			if(errno==EIDRM ||errno==EINVAL){
				exit(EXIT_SUCCESS);
			}
			if(errno==EIDRM ||errno==EINVAL){
				/*dprintf(1,"Vado in wait()");
				wait(NULL);
				dprintf(1,"Esco dalla wait in wait()");
				msgq_id=get_msgq(PATHNAME);*/
				continue;
			}
		}else{
			sono_scoria=0;
			//DONE:funzione che determina quanta energia assorbire
			energia_liberata=0;
			type=message.my_pid;
			send_message(PATHNAME,type,sono_scoria,energia_liberata);
			
		}
		//dprintf(1,"[INIBITORE]isScoria %d,Energia assorbita=%d\n",sono_scoria,energia_liberata);
		//}else{
			//mando l'inibitore in wait
			//errno = ENOMSG; // TEST
		//}

		//TODO
		/*
			Atomi guardano un semaforo che viene aggiornato dal master 
			per capire se usare o meno l'inibitore

			l'inibitore non distrugge la coda dei messaggi
			la puliamo mentre gli atomi non usano l'inibitore
		*/
		
		
		
		
			
		
		
	}

}

int rand_split(){
	int res = 0;
	int num = rand_generator(1,14); //Potrebbe cambiare
	if(num !=1){
		res = 1; // Split
	}
	return res;
}

int rand_energy(int energy_raw){
	return (energy_raw/100)*rand_generator(50, 90);
}

static void sigHandler(int signum){
	if(signum == SIGUSR1){ // Wait inibitore
		dprintf(1,"[INIBITORE]L'inibitore è ripartito\n");
	}else if(signum == SIGTERM){ // Terminazione inibitore
#ifdef DEBUG
	dprintf(1,YEL"[DINIBITORE]L'inibitore %d ha finito la sua esecuzione\n"RESET,getpid());
#endif
		exit(EXIT_SUCCESS);
	}
	if(signum==SIGINT){
		if(isActivated==1){
			isActivated=0;
			dprintf(1,"[Inibitore]Coda di messaggi Disattivata\n");
		}else if(isActivated==0){
			isActivated=1;
			dprintf(1,"[Inibitore]Coda di messaggi Attivata\n");
		}
	}
}

//TODO: file log con operazioni di inibizione per ogni secondo