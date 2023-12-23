#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Funzione di gestione del segnale
void signal_handler(int signo) {
    if (signo == SIGUSR1) {
        printf("Ricevuto SIGUSR1 dal processo figlio\n");
    }
}

int main(int argc, char const *argv[]){
    /* code */
    return 0;
}
