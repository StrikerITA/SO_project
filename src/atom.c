#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>

#define MIN_N_ATOM 2
// * Sostituire printf con write

// New Atom
pid_t split(int N_ATOM){
    pid_t pid, father_pid, child_pid;
    child_pid = fork();
	pid = getpid();
	father_pid = getppid();

    printf("PID=%6d, father_PID=%6d, child_PID=%d\n", pid, father_pid, child_pid);
    return father_pid;
}

// Signal Handling
void signal_handler(int signal) {
    if (signal == SIGUSR1) {
        printf("Ricevuto SIGUSR1 dal processo figlio\n");
    }
}

int calc_energy_free(int N_ATOM_P, int N_ATOM_C);

int main(int argc, char const *argv[]){
    int N_ATOM = atoi(argv[1]); //numero atomico
    int test_num_atom = 0;
    
    pid_t pid, father_pid, child_pid;
    printf("Num Atomico: %d\n", N_ATOM);

    // Signal Handling
    if (signal(SIGUSR1, signal_handler) == SIG_ERR) {
        perror("Errore nell'installazione della gestione del segnale");
        exit(EXIT_FAILURE);
    }

    if (N_ATOM < MIN_N_ATOM){
        //add +1 to scorie
        exit(EXIT_FAILURE);
    }
    
    // TODO: scissione atomo con una fork
    father_pid = split(N_ATOM);

    if (child_pid == 0) {
        // Codice eseguito dal processo figlio
        printf("Processo figlio avviato. PID: %d\n", getpid());

        // add some code

        // Send signal to father
        kill(getppid(), SIGUSR1);

        exit(EXIT_SUCCESS);
    } else {
        // Father's Code
        printf("Processo padre. PID: %d\n", getpid());

        // waiting signals from child
        printf("Il processo padre attende il segnale...\n");
        pause();  //waiting

        printf("Il processo padre ha ricevuto il segnale. Termina.\n");

        // Attendi la terminazione del processo figlio
        wait(NULL);

        exit(EXIT_SUCCESS);
    }
    
    // TODO: numero atomico con generazione casuale
    test_num_atom = rand();

    // TODO: somma N_ATOM_P + N_ATOM_C = numero atomico padre pre scissione


    // TODO: Calcolo ENERGY_FREE


}