#include "atom.h"

#define MIN_N_ATOMICO 2
// TODO:  Sostituire printf con write
// ! Avvio test_atomo 
// ! zsh: bus error  ./test.out
char buffer[100];
int str = 0;

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

// New Atom
pid_t split(char const *argv[]){
    pid_t pid, father_pid, child_pid;
    child_pid = fork();
	pid = getpid();
	father_pid = getppid();

    str = snprintf(buffer, sizeof(buffer), "PID=%6d, father_PID=%6d, child_PID=%d\n", pid, father_pid, child_pid);
    write(STDOUT_FILENO, buffer, str);
    
    return child_pid;
}

// Signal Handling
void signal_handler(int signal) {
    if (signal == SIGUSR1) {
        printf("Ricevuto SIGUSR1 dal processo figlio\n\n");
    }
}

// Random num generator for new atoms to be created
int rand_num_atom(int x, int y) {
    srand(time(0));
    return (rand() % (y - x + 1)) + x;
}

int calc_energy_free(int N_ATOM_P, int N_ATOM_C);

int main(int argc, char const *argv[]){
    pid_t pid;
    int N_ATOM = atoi(argv[1]); //numero atomico
    //N_ATOM = 10;
    int test_num_atom = 0;
    
    int str = snprintf(buffer, sizeof(buffer), "Num atomico %d\n", N_ATOM);
    write(STDOUT_FILENO, buffer, str);

    // Signal Handling
    if (signal(SIGUSR1, signal_handler) == SIG_ERR) {
        perror("Errore nell'installazione della gestione del segnale");
        exit(EXIT_FAILURE);
    }

    if (N_ATOM < MIN_N_ATOMICO){
        //add +1 to scorie
        exit(EXIT_FAILURE);
    }
    
    // TODO: scissione atomo con una fork
    pid = split(argv);
    
    if(pid < 0){
        perror("fork failed");
    }else if (pid == 0) {
        // Codice eseguito dal processo FIGLIO
        test_num_atom = rand_num_atom(MIN_N_ATOMICO, N_ATOM);

        char **copy = {NULL};
        char buf[6]; //bisogna caricare il num_atom su un buffer vuoto
        copy = copy_argv(argc, argv); // copio argv dentro copy
        sprintf(buf, "%d", test_num_atom); // metto un num atom randomico dentro test_num_atom
        copy[1] = buf; // metto dentro "copy" il numero atomico generato
        
        str = snprintf(buffer, sizeof(buffer), "Num Atomico %s | PID figlio %d\n", copy[1], getpid());
        write(STDOUT_FILENO, buffer, str);
        sleep(3); // TODO: mossa preventiva, da sostituire in futuro con un timer
        char *envp[] = {NULL};
        execve("./atom.out", copy, envp);

        // add some code

        // Send signal to father
        kill(getppid(), SIGUSR1);
        free(copy);
        exit(EXIT_SUCCESS);
    } else if (pid > 0){
        // Codice eseguito dal processo PADRE
        str = snprintf(buffer, sizeof(buffer), "\nProcesso padre. PID: %d\n", getpid());
        write(STDOUT_FILENO, buffer, str);

        // waiting signals from child
        str = snprintf(buffer, sizeof(buffer), "Il processo padre attende il segnale...\n\n");
        write(STDOUT_FILENO, buffer, str);
        pause();  //waiting

        str = snprintf(buffer, sizeof(buffer), "Il processo padre ha ricevuto il segnale. Termina.\n");
        write(STDOUT_FILENO, buffer, str);

        // Attendi la terminazione del processo figlio
        wait(NULL);

        exit(EXIT_SUCCESS);
    }
    
    // TODO: numero atomico con generazione casuale
    test_num_atom = rand_num_atom(MIN_N_ATOMICO, N_ATOM);
    printf("%d", test_num_atom);

    // TODO: somma N_ATOM_P + N_ATOM_C = numero atomico padre pre scissione


    // TODO: Calcolo ENERGY_FREE

    return 0;
}
