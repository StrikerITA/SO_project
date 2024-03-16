#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lib/ipc_manager.h"
#define PATHNAME "Makefile"

int main(int argc, char * argv[]){
	int sem_id=sem_get(PATHNAME);
	dprintf(1,"%s\n",argv[0]);
	dprintf(1,"%s\n",argv[1]);
	dprintf(1,"%s\n",argv[2]);
	dprintf(1,"[ALIMENTATORE]L'alimentatore %d e stato creato\n",getpid());
	
	sem_reserve(sem_id,SEM_READY);
	wait_to_zero(sem_id,SEM_READY);
	dprintf(1,"[ALIMENTATORE]start\n");


	sleep(6);
	dprintf(1,"[ALIMENTATORE]L'alimnetatore %d a finito la sua esecuzione\n",getpid());
}
