#ifndef MACROS_H
#define MACROS_H

#include <errno.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Grazie Professore
#define PRINT_ERRNO fprintf(stderr, "[%d] %s:%d: Errore #%3d \"%s\"\n", getpid(), __FILE__, __LINE__, errno, strerror(errno));
#define PRINT_ERRNO_EXIT(e) {PRINT_ERRNO exit(e);}
#define PRINT_IF_ERRNO_EXIT(e) if(errno) {PRINT_ERRNO_EXIT(e)}
#define PRINT_ERROR(x) fprintf(stderr, "[%d] %s:%d: %s",getpid(),  __FILE__, __LINE__, x);
#define PRINT_ERROR_EXIT(x, e) PRINT_ERROR(x) exit(e);
#define TEST_IPC_ACCESS(errno) {};

#endif