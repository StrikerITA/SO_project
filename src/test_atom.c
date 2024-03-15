#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void new_atom(char *argv[]){
    // Percorso del programma da eseguire
    char *programPath = "./atom.out";

    // Argomenti del programma
    //char *args[] = {"atom", "10"};
    printf("%s", argv[1]);

    // Esegui atom passando i dati con args
    if (execvp(programPath, argv) == -1) {
        perror("Errore nell'esecuzione del programma");
        exit(EXIT_FAILURE);
    }

    //invio segnale all'atomo di scindersi
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

int main(int argc, char *argv[]){
    printf("Avvio test_atomo \n");
    printf("%s", argv[1]);
    new_atom(argv);
    //test_system();
    return 0;
}