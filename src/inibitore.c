#include "lib/ipc_manager.h"
#include "lib/read_settings.h"
#include "lib/utils.h"
#include <string.h>
#include <signal.h>
#include <errno.h>

#define PATHNAME "Makefile"

int main(/*int argc, char *argv[]*/){
	int msgq_id=get_msgq(PATHNAME);
	int sem_id=sem_get(PATHNAME);
	
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
		//TODO: verificare errno
		
		if(message.info ==-1){
			// info della risposta sara (0/1)
			info=message.info;
			//TODO:funzione che determina se scinde o meno
			type=message.my_pid;
		}else{
			//TODO:funzione che determina quanto scindere
			info=message.info;
			
			type=message.my_pid;
			//info della risposta sara un valore
		}
		send_message(PATHNAME,type,info);
	}

}

int rand_split(){
	int res = 0;
	int num = rand_generator(1,4); //Potrebbe cambiare
	if(num != 1){
		res = 1; // Split
	}
	return res;
}