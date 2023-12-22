#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void signal_split(); //comunica a 1 || più atomi di splittarsi

int main(int argc, char const *argv[]){
    
    signal_split();

    return 0;
}

void signal_split(){
    // * send signal to Master, it send signal to Atom
}
