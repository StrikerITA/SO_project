#include "lib/ipc_manager.h"
#include "lib/read_settings.h"
#include "lib/utils.h"
#include <string.h>

#define PATHNAME "Makefile"
void stat_reset(statistic *stats);
void print_stats(statistic *stats);
//Return the pid of the children
int create_process(char *pathname,char * argv[]);

int main(){
	//Leggo impostazioni
	verify_file("opt.conf");
	settings_info settings=readSettings();
	printSettings(settings);

	//Dovrebbe essere NUM_ATOM+3/4
	//3 master+alimentatore+1atomo
	int num_of_process=settings.n_atom_init+3;

	//Creo Semaforo e lo inizializzo
	int sem_id,shmem_id;
	sem_id=sem_create(PATHNAME);
	printf("Semaforo Creato con id: %d\n",sem_id);
	sem_set_val(sem_id,SEM_READY,num_of_process);
	sem_set_val(sem_id,SEM_ACTIVATOR,0);
	sem_set_val(sem_id,SEM_STAT,1);

	//Creo Memoria Condivisa e la inizializzo
	shmem_id=create_shmem(PATHNAME);
	printf("Memoria Condivisa con id: %d\n",shmem_id);

	//Si attacca alla memoria condivisa, e inizializza le statistiche a zero
	statistic *stats=attach_memory_block(PATHNAME);
	bzero(stats,sizeof(statistic));
	print_stats(stats);
	
	pid_t alimentatore,atomo,attivatore;
	char process_name[20];
	char param1[20];
	char param2[20];
	char param3[20];
	char *args[5];
	//Creazione alimentatore
	strcpy(process_name,"alimentatore");
	sprintf(param1,"%d",settings.step);
	sprintf(param2,"%d",settings.n_nuovi_atomi);
	args[0]=process_name;
	args[1]=param1;
	args[2]=param2;
	args[3]=NULL;
	args[4]=NULL;
	alimentatore=create_process(process_name,args);
	
	//Creazione attivatore
	strcpy(process_name,"attivatore");
	sprintf(param1,"%d",settings.n_nuovi_atomi);

	args[0]=process_name;
	args[1]=NULL;
	args[2]=NULL;
	args[3]=NULL;
	args[4]=NULL;
	attivatore=create_process(process_name,args);

	//Creazione atomo

	strcpy(process_name,"atom");
	
	//TODO: da calcolare con funzione randomica
	int num_atomic=5;
	sprintf(param1,"%d",num_atomic);
	args[0]=process_name;
	args[1]=param1;
	args[2]=NULL;
	args[3]=NULL;
	args[4]=NULL;
	atomo=1;
	for(int i=0;i<settings.n_atom_init && atomo>0;i++){
		atomo=create_process(process_name,args);
	}
	
	
	
	//!Le Preparazioni sono Pronte
	sem_reserve(sem_id,SEM_READY);
	wait_to_zero(sem_id,SEM_READY);
	dprintf(1,"La simulazione  e iniziata\n");
	int energia_disponibile;
	for(int i=0;i<settings.sim_duration;i++){
		sleep(1);
		sem_reserve(sem_id,SEM_STAT);
		stats->q_energia_consumata_tot+=settings.energy_demand;
		
		
		energia_disponibile=stats->q_energia_prodotta_tot-stats->q_energia_consumata_tot;
		if(energia_disponibile<settings.energy_demand){
			dprintf(1,"blackout\n");
			//TODO: DA finire gestione errore
		}
		stats->q_energia_consumata_sec+=settings.energy_demand;
		
		print_stats(stats);
		stat_reset(stats);

		sem_release(sem_id,SEM_STAT,1);
	}

	

	detach_memory_block(stats);
	sem_destroy(PATHNAME);
	printf("Semaforo Rimosso\n");
	destroy_memory_block(PATHNAME);
	printf("Memoria Condivisa Rimossa\n");

}

void stat_reset(statistic *stats){
	stats->n_attivazioni_sec=0;
	stats->n_scissioni_sec=0;
	stats->q_energia_prodotta_sec=0;
	stats->q_energia_consumata_sec=0;
	stats->n_scorie_sec=0;
}

void print_stats(statistic *stats){
	//TODO:Da modificare non va bene il printf sostituirla con una stampa non bufferizzata
	printf(//my_message,
		"================Statistiche===============\n"
		"Numero attivazioni totali: %d\n"
		"Numero attivazioni al secondo: %d\n"
		"Numero scissioni totali: %d\n"
		"Numero scissioni al secondo: %d\n"
		"Quantita energia prodotta: %d\n"
		"Quantita energia prodotta al secondo: %d\n"
		"Quantita energia consumata: %d\n"
		"Quantita energia consumata al secondo: %d\n"
		"Numero scorie totali: %d\n"
		"Numero scorie nel ultimo secondo: %d\n"
		"==========================================\n",
		stats->n_attivazioni_tot,
		stats->n_attivazioni_sec,
		stats->n_scissioni_tot,
		stats->n_scissioni_sec,
		stats->q_energia_prodotta_tot,
		stats->q_energia_prodotta_sec,
		stats->q_energia_consumata_tot,
		stats->q_energia_consumata_sec,
		stats->n_scorie_tot,
		stats->n_scorie_sec
	);
	//printf("%s",my_message);
}

int create_process(char *pathname,char * argv[]){
	pid_t pid;
	
	pid=fork();
	if(pid==-1){
		//!Error Meltdown
		printf("Meltdown");
	}else if(pid==0){
		execve(pathname,argv,NULL);
	}
	return pid;

}