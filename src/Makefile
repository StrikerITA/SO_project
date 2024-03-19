CFLAGS= -Wvla -Wextra -Werror
all:
	gcc $(CFLAGS) master.c lib/ipc_manager.c lib/read_settings.c lib/utils.c -o master.out
	gcc $(CFLAGS) atom.c lib/utils.c lib/ipc_manager.c -o atom.out                
	gcc $(CFLAGS) AlimentatoreStupido.c lib/utils.c lib/ipc_manager.c -o alimentatore.out            
	gcc $(CFLAGS) AttivatoreStupido.c lib/utils.c lib/ipc_manager.c -o attivatore.out 

clear: 
	rm master.out atom.out attivatore.out alimentatore.out