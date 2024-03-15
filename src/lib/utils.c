#include "utils.h"

int isNumber(char s[]){
    for (int i = 0; s[i]!= '\0'; i++){
        if(isdigit(s[i]) == 0)
              return 0;
    }
    return 1;
}

// Random num generator for new atoms to be created
int rand_num_atom(int min, int max){
    srand(getpid());
    return (rand() % (max - min + 1)) + min;
}