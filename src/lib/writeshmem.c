#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shared_memory.h"

#define BLOCK_SIZE 4096
#define NUM_ITERATIONS 10

int main(int argc,char* argv[]){
    if(argc !=2){
        printf("usage - %s [stuff to write]", argv[0]);
        return -1;
    }

    //grab the shared memory block
	char *block =attach_memory_block(FILENAME,BLOCK_SIZE);

	if(block==NULL){
		printf("ERROR:couldn't get block \n");
		return -1;
	}
	int num_writes=5;

	for(int i=0;i<NUM_ITERATIONS;i++){
		//scrivo in memoria
			printf("Writing: \"%s\"\n",argv[1]);
			//strncpy(block,argv[1],BLOCK_SIZE);
			strncpy(block,argv[1],BLOCK_SIZE);// copia in block la stringa da inserire
	}
	

	//scrivo in memoria
	printf("Writing: \"%s\"\n",argv[1]);
	//strncpy(block,argv[1],BLOCK_SIZE);
	strncpy(block,argv[1],BLOCK_SIZE);// copia in block la stringa da inserire

	

	
	detach_memory_block(block);
	return 0;
}