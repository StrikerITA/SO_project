CFLAGS= -Wvla -Wextra -Werror -D_GNU_SOURCE
DEPENDENCIES=lib/ipc_manager.c lib/read_settings.c lib/utils.c

BINARY= master.out atom.out alimentatore.out attivatore.out inibitore.out



all: master atom alimentatore attivatore inibitore

master:
	gcc $(CFLAGS) master.c $(DEPENDENCIES)   -o master.out 
atom:
	gcc $(CFLAGS) atom.c $(DEPENDENCIES) -o atom.out 
alimentatore:
	gcc $(CFLAGS) alimentatore.c $(DEPENDENCIES) -o alimentatore.out 
attivatore:
	gcc $(CFLAGS) attivatore.c $(DEPENDENCIES) -o attivatore.out 
inibitore:
	gcc $(CFLAGS) inibitore.c $(DEPENDENCIES) -o inibitore.out 


debug: dmaster datom dalimentatore dattivatore

d_no_atom:dmaster atom dalimentatore dattivatore dinibitore

dmaster:
	gcc $(CFLAGS) master.c $(DEPENDENCIES)  -o master.out -DDEBUG
datom:
	gcc $(CFLAGS) atom.c $(DEPENDENCIES) -o atom.out -DDEBUG
dalimentatore:
	gcc $(CFLAGS) alimentatore.c $(DEPENDENCIES) -o alimentatore.out -DDEBUG
dattivatore:
	gcc $(CFLAGS) attivatore.c $(DEPENDENCIES) -o attivatore.out -DDEBUG
dinibitore:
	gcc $(CFLAGS) inibitore.c $(DEPENDENCIES) -o inibitore.out -DDEBUG


clear: 
	rm *.out


