#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void new_atom(){
    // * send signal to master ???

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
    // TODO: ogni x nanosecondi crea nuovi atomi
    int flag = 0;
    char* str = "Avvio PSU... \n";
    write(1, str, strlen(str));
    
    while(flag == 0){
        str = "Creazione nuovo atomo.\n\n";
        write(1, str, strlen(str));
        //new_atom();
        test_system();
        sleep(3); // Per ora proviamo sleep
    }

    return 0;
}