#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024 // Dimensione dell'area di memoria condivisa
#define MIN_N_ATOMICO 2
#define MAX_CHILD_PROCESSES 3
int test = 0;

int lunghezza_numero(int numero);
char **copy_argv(int argc, char const *argv[]);
void split(int argc, char const *argv[], char *shared_mem);
void signal_handler(int signal);
int calc_energy_free(int N_ATOM_P, int N_ATOM_C);
