#include "utils.h"

int isNumber(char s[]){
    for (int i = 0; s[i]!= '\0'; i++){
        if(isdigit(s[i]) == 0)
              return 0;
    }
    return 1;
}

// Random num generator for new atoms to be created
int rand_num_atom(int min, int max){
    return (rand() % (max - min + 1)) + min;
}


int create_process(char *pathname,char * argv[],pid_t meltdownSig){
	pid_t pid;
	
	pid=fork();
	if(pid==-1){
		//!Error Meltdown
		kill(meltdownSig,SIGINT);
		printf("Meltdown");
		exit(EXIT_SUCCESS);
	}else if(pid==0){
		execve(pathname,argv,NULL);
		exit(EXIT_SUCCESS);
		
	}
	return pid;
}