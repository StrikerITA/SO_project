#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>

int num_atom_generator(int min, int max);
int num_atom_generator_v2(int num);
int isNumber(char s[]);
int rand_num_atom(int min, int max);
int create_process(char *pathname,char * argv[],pid_t meltdownSig);