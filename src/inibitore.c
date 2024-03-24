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

int main(int argc, char *argv[]){
	// filtrare errori argv

	int num_param=argc;
	char *mia_variabile=argv[0];


	int msgq_id=get_msgq(PATHNAME);
	int sem_id=sem_get(PATHNAME);
	statistic *stats=attach_memory_block(PATHNAME);

	signal(SIGUSR1, sigHandler);
	signal(SIGTERM, sigHandler);
	
	struct msgbuff message;

	int type;
	pid_t pid;
	int info;

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
		message=receive_message(PATHNAME,1);
		if(errno==EIDRM ||errno==EINVAL){
			wait(NULL);
			msgq_id=get_msgq(PATHNAME);
			continue;
		}
		//DONE: verificare errno
		
		if(message.info ==-1){
			// info della risposta sara (0/1)
			//DONE:funzione che determina se scinde o meno
			info=rand_split();
		}else{
			//DONE:funzione che determina quanta energia assorbire
			info=rand_energy(message.info);
			sem_reserve(sem_id,SEM_STATS);

			if(errno==EIDRM ||errno==EINVAL){
				exit(EXIT_SUCCESS);
			}
			stats->q_energia_assorbita+=info;		
			sem_release(sem_id,SEM_STATS,1);
			if(errno==EIDRM ||errno==EINVAL){
				exit(EXIT_SUCCESS);
			}
		}
		type=message.my_pid;
		send_message(PATHNAME,type,info);
		if(errno==EIDRM ||errno==EINVAL){
			wait(NULL);
			msgq_id=get_msgq(PATHNAME);
			continue;
		}
	}

}

int rand_split(){
	int res = 0;
	int num = rand_generator(1,4); //Potrebbe cambiare
	if(num == 1){
		res = 1; // Split
	}
	return res;
}

int rand_energy(int energy_raw){
	return (energy_raw/100)*rand_generator(10, 90);
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
}

//TODO: file log con operazioni di inibizione per ogni secondo