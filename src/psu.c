#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void new_atom();

int main(int argc, char const *argv[]){
    // TODO: ogni x nanosecondi crea nuovi atomi

    new_atom();

    return 0;
}

void new_atom(){
    // * send signal to master ???
}