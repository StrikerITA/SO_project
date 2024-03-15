#include "atom.h"
#include "utils.h"

char buffer[100];
char num_buffer[50];
key_t key = 34197;
pid_t child_processes[MAX_CHILD_PROCESSES];
int num_child_processes = 0, str = 0;
//todo: impostare @param num_child_processes = 0 solo alla prima esecuzione

int lunghezza_numero(int numero) {
    int lunghezza = 0;
    
    // Gestione del numero negativo
    if(numero < 0) {
        lunghezza++; // Conta il segno '-'
        numero = -numero; // Converti il numero in positivo per il conteggio delle cifre
    }
    
    // Conteggio delle cifre del numero
    do {
        lunghezza++;
        numero /= 10;
    } while (numero != 0);
    
    return lunghezza;
}

char **copy_argv(int argc, char const *argv[]) {
    // Allocazione di memoria per il nuovo vettore
    char **new_argv = (char **)malloc((argc + 1) * sizeof(char *));
    if (new_argv == NULL) {
        perror("Errore durante l'allocazione di memoria");
        exit(EXIT_FAILURE);
    }

    // Copia degli elementi di argv[] nel nuovo vettore
    for (int i = 0; i < argc; i++) {
        int length = strlen(argv[i]) + 1; // Calcolo della lunghezza della stringa, incluso il terminatore NULL
        new_argv[i] = (char *)malloc(length * sizeof(char)); // Allocazione di memoria per la singola stringa
        if (new_argv[i] == NULL) {
            perror("Errore durante l'allocazione di memoria per una stringa");
            exit(EXIT_FAILURE);
        }
        strcpy(new_argv[i], argv[i]); // Copia della stringa
    }

    new_argv[argc] = NULL; // Imposta l'ultimo elemento del vettore a NULL come terminatore

    return new_argv;
}

// Funzione per accedere ad un'area di memoria condivisa
char *accedi_memoria_condivisa(key_t chiave) {
    int id_area_memoria;

    // Ottenere l'ID dell'area di memoria condivisa
    id_area_memoria = shmget(chiave, SHM_SIZE, 0666 | IPC_CREAT);
    if (id_area_memoria == -1) {
        perror("Errore durante la creazione dell'area di memoria condivisa");
        exit(EXIT_FAILURE);
    }

    // Collegare l'area di memoria condivisa al processo
    char *area_memoria = shmat(id_area_memoria, NULL, 0);
    if (area_memoria == (char *)-1) {
        perror("Errore durante l'attacco all'area di memoria condivisa");
        exit(EXIT_FAILURE);
    }

    return area_memoria;
}

// New Atom
void split(int argc, char const *argv[], char* shared_mem){
    pid_t pid, father_pid, child_pid;

    if (num_child_processes >= MAX_CHILD_PROCESSES) {
        str = snprintf(buffer, sizeof(buffer), "Numero massimo di processi figli raggiunto\n");
        write(STDOUT_FILENO, buffer, str);
        exit(SIGINT);
        return;
    }

    int N_ATOM = atoi(argv[1]); //numero atomico
    child_pid = fork();
	pid = getpid();
	father_pid = getppid();

    str = snprintf(buffer, sizeof(buffer), "PID=%6d, father_PID=%6d, child_PID=%d\n", pid, father_pid, child_pid);
    write(STDOUT_FILENO, buffer, str);
    
    if(child_pid < 0){
        //inviare segnale a master FORK FAILED - SIGURS1
        perror("fork failed");
    }else if (child_pid == 0) {
        // Codice eseguito dal processo FIGLIO
        child_processes[num_child_processes] = pid;
        num_child_processes++; // !!! NON INCREMENTA STA VARIABILE
        //mandare valore al padre prima di fare execve
        sprintf(num_buffer, "%d", num_child_processes);
        shared_mem = num_buffer;

        // Il figlio diventa un nuovo PADRE
        int test_num_atom = rand_num_atom(1, N_ATOM);

        char **copy = {NULL};
        char buf[lunghezza_numero(N_ATOM)]; //bisogna caricare il num_atom su un buffer vuoto
        copy = copy_argv(argc, argv); // copio argv dentro copy
        sprintf(buf, "%d", test_num_atom); // metto un num atom randomico dentro buf
        copy[1] = buf; // metto dentro "copy" il numero atomico generato
        
        //*buffer = "";
        str = snprintf(buffer, sizeof(buffer), "Num Atomico %s | PID figlio %d\n", copy[1], getpid());
        write(STDOUT_FILENO, buffer, str);
        sleep(3); // TODO: mossa preventiva, da sostituire in futuro con un timer
        char *envp[] = {NULL};
        execve("./atom.out", copy, envp);

        // Send signal to father
        kill(getppid(), SIGUSR1);
        free(copy);
        exit(EXIT_SUCCESS);
    } else {
        // Codice eseguito dal processo PADRE
        str = snprintf(buffer, sizeof(buffer), "\nProcesso padre | PID: %d\n", getpid());
        write(STDOUT_FILENO, buffer, str);

        //memoria condivisa
    }

}

// Signal Handling
void signal_handler(int signal) {
    switch (signal){
        case SIGUSR1: {
            str = snprintf(buffer, sizeof(buffer), "Ricevuto SIGUSR1 dal processo figlio\n\n");
            write(STDOUT_FILENO, buffer, str);
            break;
        }

        case SIGINT: {
            str = snprintf(buffer, sizeof(buffer), "\nPulizia dello stack e dei processi in corso\n");
            write(STDOUT_FILENO, buffer, str);

            // Termina tutti i processi figli
            for (int i = 0; i < num_child_processes; i++) {
                kill(child_processes[i], SIGTERM);
            }

            // Attendi che tutti i processi figli terminino
            for (int i = 0; i < num_child_processes; i++) {
                wait(NULL);
            }

            // Termina il programma
            exit(EXIT_SUCCESS);
            break;
        }
            
        default:{
            str = snprintf(buffer, sizeof(buffer), "Segnale non riconosciuto dall'handler\n");
            write(STDOUT_FILENO, buffer, str);
            break;
        }
    }
}

int calc_energy_free(int N_ATOM_P, int N_ATOM_C);

int main(int argc, char const *argv[]){
    // Todo: crea processi solo se ricevi il segnale di scissione
    //prima esecuzione -> carica in "shared_mem" = num_child_processes
    
    // Accedere all'area di memoria condivisa
    char *shared_mem = accedi_memoria_condivisa(key);
    int N_ATOM = atoi(argv[1]);
    int test_num_atom = 0;
    
    if (test == 0){
        strcpy(shared_mem, "0");
        test++;
    }

    while (1){
        signal(SIGINT, signal_handler);
        num_child_processes = atoi(shared_mem);

        if (num_child_processes == 0){
            str = snprintf(buffer, sizeof(buffer), "\nPrimo processo padre | PID: %d\n", getpid());
            write(STDOUT_FILENO, buffer, str);
        }

        // Signal Handling
        if (signal(SIGUSR1, signal_handler) == SIG_ERR) {
            perror("Errore nell'installazione della gestione del segnale");
            exit(EXIT_FAILURE);
        }
        
        if (N_ATOM <= MIN_N_ATOMICO){
            // TODO: add +1 to scorie
            exit(EXIT_FAILURE);
        }

        // Scrivo sull'area di memoria condivisa
        sprintf(buffer, "%d", num_child_processes);
        strcpy(shared_mem, buffer);
        printf("%d", num_child_processes);
    }

    // Dettach l'area di memoria condivisa dal processo
    if (shmdt(shared_mem) == -1) {
        perror("Errore durante il detach dall'area di memoria condivisa");
        exit(EXIT_FAILURE);
    }

    // devo controllare il semaforo SEM_ACTIVATOR per splittare o meno l'atomo
    split(argc, argv, shared_mem);

    // TODO: somma N_ATOM_P + N_ATOM_C = numero atomico padre pre scissione
    // TODO: Calcolo ENERGY_FREE
    return 0;
}

/*
Supera parte di inizializzazione salvandosi il numero atomico
richiede scissione SEMAFORO

quando ce l'ha verifica che l'atomo non sia una scoria
se e scoria comunica alle statistiche e termina
altrimenti
calcola il numero atomico del figlio e l'energia liberata
si scinde
-----> FIGLIO:si avvia un nuovo atomo e si comunica il suo figlio atomico <-----
PADRE: aggiorna il numero atomico
comunica alle statistiche la scissione 
richiede scissione e si ritorna alla riga del commento 7

*/