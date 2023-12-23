#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char const *argv[]){
    printf("Avvio test_atomo \n");
    // Percorso del programma da eseguire
    char *programPath = "./atom.out";

    // Argomenti del programma
    char *args[] = {"atom", "10", NULL};

    // Esegui atom passando i dati con args
    if (execvp(programPath, args) == -1) {
        perror("Errore nell'esecuzione del programma");
        exit(EXIT_FAILURE);
    }

    // Questo codice non verrà mai eseguito a causa della chiamata a execvp
    printf("Questo messaggio non verrà mai stampato.\n");

    
    return 0;
}