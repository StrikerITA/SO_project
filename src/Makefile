CFLAGS= -Wvla -Wextra -Werror
DEPENDENCIES=lib/ipc_manager.c lib/read_settings.c lib/utils.c
all:
	gcc $(CFLAGS) master.c $(DEPENDENCIES)  -o master.out
	gcc $(CFLAGS) atom.c $(DEPENDENCIES) -o atom.out                
	gcc $(CFLAGS) alimentatore.c $(DEPENDENCIES) -o alimentatore.out            
	gcc $(CFLAGS) attivatore.c $(DEPENDENCIES) -o attivatore.out 

clear: 
	rm *.out