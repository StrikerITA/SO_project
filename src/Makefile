CFLAGS= -Wvla -Wextra -Werror
all:
	gcc $(CFLAGS) master.c lib/ipc_manager.c lib/read_settings.c lib/utils.c -o master
	gcc $(CFLAGS) atom.c lib/utils.c lib/ipc_manager.c -o atom                     
	gcc $(CFLAGS) AlimentatoreStupido.c lib/utils.c lib/ipc_manager.c -o alimentatore             
	gcc $(CFLAGS) AttivatoreStupido.c lib/utils.c lib/ipc_manager.c -o attivatore 