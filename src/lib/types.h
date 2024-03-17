

typedef struct{
	//Versione "minima"
	//attivatore
	int n_attivazioni_tot;
	int n_attivazioni_sec;
	//atomi al scindersi
	int n_scissioni_tot;
	int n_scissioni_sec;
	//atomo  
	int q_energia_prodotta_tot;
	int q_energia_prodotta_sec;

	//master al consumare energia
	int q_energia_consumata_tot;
	int q_energia_consumata_sec;

	//atomo al diventare scoria
	int n_scorie_tot;
	int n_scorie_sec;
	//versione "normal"
	//int q_energia_assorbita;

}statistic;

typedef enum{
	SEMAPHORE=0,
	SHARED_MEMORY,
	MESSAGE_QUEUE
}ipc_id;
typedef enum{
	SEM_READY=0,
	SEM_ACTIVATOR=1,
	SEM_STATS=2
}sem_types;
