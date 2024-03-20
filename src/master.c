#include "lib/ipc_manager.h"
#include "lib/read_settings.h"
#include "lib/utils.h"
#include <string.h>
#include <signal.h>

#define PATHNAME "Makefile"
void stat_reset(statistic *stats);
void print_stats(statistic *stats);
//Return the pid of the children
void end();
static void sigHandler(int signum);

pid_t alimentatore;

int main(int argc, char * argv[]){
	char *path;
	srand(getpid());
	//TODO:Finire gestione se trova o meno il file
	
	if (argc < 2){
		path = "opt.conf";
	}else{
		path = argv[1];
	}
	
	verify_file(path);

	signal(SIGTERM,sigHandler);
	signal(SIGALRM,sigHandler); 
	signal(SIGUSR1,sigHandler); //Explode
	signal(SIGINT,sigHandler); //CTRL + C
	
	settings_info settings=readSettings(path);
	printSettings(settings);

	//Dovrebbe essere NUM_ATOM+3/4
	//3 master+alimentatore+1atomo
	int num_of_process=settings.n_atom_init+3;

	//Creo Semaforo e lo inizializzo
	int sem_id,shmem_id;
	sem_id=sem_create(PATHNAME);
	//dprintf(1,"Semaforo Creato con id: %d\n",sem_id);
	sem_set_val(sem_id,SEM_READY,num_of_process);
	sem_set_val(sem_id,SEM_ACTIVATOR,0);
	sem_set_val(sem_id,SEM_STATS,1);

	//Creo Memoria Condivisa e la inizializzo
	shmem_id=create_shmem(PATHNAME);
	//dprintf(1,"Memoria Condivisa con id: %d\n",shmem_id);

	//Si attacca alla memoria condivisa, e inizializza le statistiche a zero
	statistic *stats=attach_memory_block(PATHNAME);
	bzero(stats,sizeof(statistic));
	//print_stats(stats);
	
	pid_t  atomo, attivatore, master;
	master=getpid();
	char process_name[20];
	char param1[20];
	char param2[20];
	char param3[20];
	char param4[20];
	char param5[20];
	char param6[20];
	char *args[8];

	//Creazione alimentatore
	strcpy(process_name,"alimentatore.out");
	sprintf(param1,"%d",settings.step);
	sprintf(param2,"%d",settings.n_nuovi_atomi);
	sprintf(param3,"%d",(int)master);
	sprintf(param4,"%d",settings.n_atom_max);
	sprintf(param5,"%d",settings.min_n_atomico);
	sprintf(param6,"%d",settings.energy_explode_threshold);
	args[0]=process_name;
	args[1]=param1;//step
	args[2]=param2;//n_nuovi_atomi
	args[3]=param3;//master pid
	args[4]=param4;
	args[5]=param5;
	args[6]=param6;
	args[7]=NULL;
	
	alimentatore=create_process(process_name,args,master);
	//dprintf(1,"PID ALIMENTATORE: %d\n",alimentatore);
	
	//Creazione attivatore
	strcpy(process_name,"attivatore.out");
	sprintf(param1,"%d",settings.n_nuovi_atomi);

	args[0]=process_name;
	args[1]=param1;
	args[2]=NULL;
	args[3]=NULL;
	args[4]=NULL;
	attivatore=create_process(process_name,args,master);
	//dprintf(1,"%d-",attivatore);

	//Creazione atomo
	strcpy(process_name,"atom.out");
	
	//TODO: da calcolare con funzione randomica
	int num_atomic=10;
	sprintf(param1,"%d",num_atomic);
	sprintf(param2,"%d",settings.min_n_atomico);
	sprintf(param3,"%d",master);
	sprintf(param4,"%d",1);
	sprintf(param5,"%d",settings.energy_explode_threshold);
	args[0]=process_name;//
	args[1]=param1;//numero atomico
	args[2]=param2;//min numero atomico
	args[3]=param3;//pid master
	args[4]=param4;//first_atom
	args[5]=param5;
	args[6]=NULL;
	atomo=1;
	for(int i=0;i<settings.n_atom_init && atomo>0;i++){
		atomo=create_process(process_name,args,master);
		//dprintf(1,"%d-",atomo);
	}
	
	// ! Le Preparazioni sono Pronte
	sem_reserve(sem_id,SEM_READY);
	//printf(1,"[MASTER]Ho prelevato 1 r\n");
	wait_to_zero(sem_id,SEM_READY);
	alarm(settings.sim_duration);
	dprintf(1,"La simulazione e iniziata\n");
	//todo: da modificare energia disponibile
	int energia_disponibile = 0;
	int energia_liberata = 0;

	while (1){
		sleep(1);
		sem_reserve(sem_id,SEM_STATS);
		energia_disponibile=stats->q_energia_prodotta_tot - stats->q_energia_consumata_tot;

		if(energia_disponibile<settings.energy_demand){
			dprintf(1,"[MASTER]Programma finito con blackout\n");
			end();
			//TODO: DA finire gestione errore
		}	
		stats->q_energia_consumata_tot+=settings.energy_demand;
		stats->q_energia_consumata_sec+=settings.energy_demand;
		
		print_stats(stats);
		stat_reset(stats);

		sem_release(sem_id,SEM_STATS,1);
	}
	
	/*for(int i=0;i<settings.sim_duration;i++){
		
	}*/
	end();
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
	dprintf(1,//my_message,
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

void end(){
	kill(alimentatore,SIGTERM);
	//detach_memory_block(stats);
	sem_destroy(PATHNAME);
	//dprintf(1,"Semaforo Rimosso\n");
	destroy_memory_block(PATHNAME);
	//dprintf(1,"Memoria Condivisa Rimossa\n");
	/*dprintf(1,"============Statistiche Finali============");
	print_stats(stats);*/
	exit(EXIT_SUCCESS);
}

static void sigHandler(int signum){
	switch (signum){
		case SIGTERM:
			dprintf(1,"[MASTER]Programma finito con meltdown\n");
			end();
			break;
		case SIGALRM:
			dprintf(1,"[MASTER]Programma finito per timeout\n");
			end();
			break;
		case SIGUSR1:
			dprintf(1,"[MASTER]Programma finito per explode\n");
			end();
			break;
		case SIGINT:
			dprintf(1,"\n[MASTER]Programma finito per SIGINT\n");
			dprintf(1,"[MASTER]Rimozione processi attivi...\n");
			end();
			break;
		default:
			dprintf(1, "Segnale non riconosciuto\n");
			break;
	}
}