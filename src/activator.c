#define _GNU_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib/shared_memory.h"
#include "lib/semaphores.h"

#define FILENAME "activator.c"

void signal_split(); //comunica a 1 || più atomi di splittarsi

int main(int argc, char const *argv[]){
    //creo memoria intera
    int *block=attach_memory_block(FILENAME,100);

    if(block==NULL){
		printf("ERROR:couldn't get block \n");
		return -1;
	}

    /*
    	TODO:creazione semaforo
    */
    
    while(true){
		/*
			capire il criterio di generazione num
			generazione num, di atomi da scindere
		*/
		int num_atom_split=4;//numero di prova

        /*
			L'attivatore e in concorrenza con gli
			attivatori 
        */

		//attivatore ha accesso alla risorsa
		block=block+4;

    }
    
    

    //suca emanuele
    signal_split();
    
    return 0;
}

void signal_split(){
    // * send signal to Master, it send signal to Atom
}
