#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MIN_N_ATOMICO 2

char **copy_argv(int argc, char const *argv[]);
pid_t split(char const *argv[]);
void signal_handler(int signal);
int rand_num_atom(int x, int y);
int calc_energy_free(int N_ATOM_P, int N_ATOM_C);
