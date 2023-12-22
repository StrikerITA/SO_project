#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MIN_N_ATOM 2

unsigned int split(int N_ATOM);
int calc_energy_free(int N_ATOM_P, int N_ATOM_C);

int main(int argc, char const *argv[]){
    int N_ATOM = 6; //numero atomico
    unsigned int pid, father_pid, child_pid;

    if (N_ATOM < MIN_N_ATOM){
        //add +1 to scorie
        exit(EXIT_FAILURE);
    }
    

    // TODO: scissione atomo con una fork
    father_pid = split(N_ATOM);
    
    // TODO: numero atomico con generazione casuale


    // TODO: somma N_ATOM_P + N_ATOM_C = numero atomico padre pre scissione


    // TODO: Calcolo ENERGY_FREE


}

unsigned int split(int N_ATOM){
    unsigned int pid, father_pid, child_pid;
    child_pid = fork();
	pid = getpid();
	father_pid = getppid();

    printf("PID=%6d, father_PID=%6d, child_PID=%d\n", pid, father_pid, child_pid);
    return father_pid;
}