#include "utils.h"

int isNumber(char s[]){
    for (int i = 0; s[i]!= '\0'; i++){
        if(isdigit(s[i]) == 0)
              return 0;
    }
    return 1;
}

// Random num generator for new atoms to be created
int rand_generator(int min, int max){
    return (rand() % (max - min + 1)) + min;
}


int create_process(char *pathname,char * argv[],pid_t meltdownSig){
	pid_t pid;
	
	pid=fork();
	if(pid==-1){
		//!Error Meltdown
		kill(meltdownSig,SIGTERM);
		//printf("Meltdown");
		exit(EXIT_SUCCESS);
	}else if(pid==0){
		execve(pathname,argv,NULL);
		exit(EXIT_SUCCESS);
		
	}
	return pid;
}

int num_atom_generator(int min, int max){
	int rand_num = rand_generator(min, max);
	if(rand_num <= max/2){
		return rand_num * rand_generator(15, 40);
	}else{
		return rand_num * rand_generator(3,15);
	}
}

int num_atom_generator_v2(int num){
	return num & rand_generator(10,100);
	/*if(rand_num <= max/2){
		return rand_num * rand_generator(15, 40);
	}else{
		return rand_num * rand_generator(3,15);
	}*/
}




// #include <stdio.h>
// #include <unistd.h>
// #include <poll.h>

// int main(){

//     struct pollfd stdin_fd = { .fd = 0, .events = POLLIN };
//     while (1) {
//         dprintf(1, "suka\n");
//         sleep(1);
//         if (poll(&stdin_fd, 1, 0) > 0) {
//             if (stdin_fd.revents & POLLIN) {
//                 char input;
//                 read(0, &input, 1);
//                 printf("Hai premuto il tasto '%c'\n", input);
//                 if (input == 'q') {
//                     printf("Hai premuto 'q'. Uscita dal programma.\n");
//                     break;
//                 }
//             }
//         }
//     }

//     return 0;
// }