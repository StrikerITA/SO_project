#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include "lib/ipc_manager.h"
#include "lib/utils.h"
#define PATHNAME "Makefile"

static void sigHandler(int signum);

int main(int argc, char * argv[]){
	srand(getpid());
	int numero=argc;
	int sem_id=sem_get(PATHNAME);

	int step=atoi(argv[1]);
	int n_nuovi_atomi=atoi(argv[2]);
	pid_t master=atoi(argv[3]);
	int n_atom_max=atoi(argv[4]);
	int min_n_atom=atoi(argv[5]);
	int energy_explode_threshold = atoi(argv[6]);
	

	pid_t atomo;
	char process_name[20];
	char param1[20];
	char param2[20];
	char param3[20];
	char param4[20];
	char param5[20];
	char *args[7];
	int num_atomic;

	signal(SIGTERM,sigHandler);
	struct timespec my_time;
	
	sem_reserve(sem_id,SEM_READY);
#ifdef DEBUG 
	dprintf(1,"[DALIMENTATORE] Aspetto avvio simulazione\n");
#endif
	wait_to_zero(sem_id,SEM_READY);
	
	while(true){
		my_time.tv_sec = 0;
		my_time.tv_nsec = step; 
		nanosleep(&my_time, NULL);
		//calcolo numero atomico
		
		atomo=1;
		for(int i=0;i<n_nuovi_atomi && atomo>0;i++){
	
			strcpy(process_name,"atom.out");
			//calcolo num_atomico
			num_atomic=rand_num_atom(1, n_atom_max);
			sprintf(param1,"%d",num_atomic);
			sprintf(param2,"%d",min_n_atom);
			sprintf(param3,"%d",master);
			sprintf(param4,"%d",0);
			sprintf(param5,"%d", energy_explode_threshold);
			args[0]=process_name;
			args[1]=param1;
			args[2]=param2;
			args[3]=param3;
			args[4]=param4;
			args[5]=param5;
			args[6]=NULL;
			atomo=create_process(process_name,args,master);
			
		}
#ifdef DEBUG
	dprintf(1,YEL"[DALIMENTATORE]Ho creato %d atomi\n"RESET,n_nuovi_atomi);
#endif
	}
}
static void sigHandler(int signum){
	if(signum==SIGTERM){
#ifdef DEBUG
	dprintf(1,YEL"[DALIMENTATORE]L'alimentatore %d ha finito la sua esecuzione\n"RESET,getpid());
#endif
		exit(EXIT_SUCCESS);
	}
}