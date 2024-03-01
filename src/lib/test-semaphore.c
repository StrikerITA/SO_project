#include <sys/ipc.h>
#include <sys/sem.h>

key_t key;
int semid;

key = ftok("/lib/Makefile", 'E');
semid = semget(key, 10, 0666 | IPC_CREAT);