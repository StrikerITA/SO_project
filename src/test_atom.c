#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void new_atom(){
    // Percorso del programma da eseguire
    char *programPath = "./atom.out";

    // Argomenti del programma
    char *args[] = {"atom", "10", NULL};

    // Esegui atom passando i dati con args
    if (execvp(programPath, args) == -1) {
        perror("Errore nell'esecuzione del programma");
        exit(EXIT_FAILURE);
    }
}

void test_system(){
    // Percorso del programma da eseguire
    char *programPath = "./atom.out";

    // Esegui atom passando i dati con args
    if (system(programPath) == -1) {
        perror("Errore nell'esecuzione del programma");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char const *argv[]){
    printf("Avvio test_atomo \n");
    
    //new_atom();
    test_system();
    return 0;
}