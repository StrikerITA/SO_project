#include "read_settings.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "utils.h"
#include <errno.h>

/*Lo scopo finale di questo file e riempire questa struttura*/
settings_info si={10,50,50,2,6,999999999,10,700000};
/*file da cui si leggera il file*/
FILE *configFile;
/*Carattere utilizzato per svolgere le operazioni in getToken()*/
char peek;
token tok;
int linea=0;


void make_default_settings_file(){
	char string_default_settings[]="#prototipo di file predefinito\n"
			"#!IMPORTANTE: questo documento deve essere nella stessa cartella che il programma chiamante\n"
			"#Quantita di energia che dovra prelevare il MASTER ogni secondo\n"
			"ENERGY_DEMAND=10;\n"
			"#Numero processi atomo che genera il MASTER\n"
			"N_ATOMI_INIT=50;\n"
			"#Massimo numero atomico di un atomo,deve essere >=1\n"
			"N_ATOMI_MAX=50;\n"
			"#Numero atomico minimo che puo avere un atomo per scindersi\n"
			"MIN_N_ATOMICO=2;\n"
			"#Durata della simulazione in nanosecondi\n"
			"SIM_DURATION=6;\n"
			"#Numero nanosecondi che aspettera l' ALIMENTATORE prima di emettere nuovi atomi\n"
			"STEP=999999999;\n"
			"#Numero di atomi che crea il processo ALIMENTATORE\n"
			"N_NUOVI_ATOMI=10;\n"
			"#Soglia massima per evitare la Terminazione di tipo blackout\n"
			"ENERGY_EXPLODE_THRESHOLD=700000;\0";

	configFile=fopen("opt.conf","w");

	fputs(string_default_settings,configFile);
	fclose(configFile);
}
/*
	Restituisce 0 se riesce ad aprire il file
	Restituisce -1 se non lo trova, e si dovra procedere a crearlo
*/
int verify_file(char *path){

	int status;
	configFile=fopen(path, "r");
	// checking if the file is opened successfully
	if (configFile == NULL) {
		dprintf(1,"File non trovato\n");
		dprintf(1,"Sto generando il file opt.conf con i valori di default\n");
		make_default_settings_file();
		status=-1;
	}else{
		fclose(configFile);
		status=0;
	}
	return status;
}

void printSettings(settings_info s){
	printf("\n"
		"=================SETTINGS=================\n"
		"ENERGY_DEMAND: \t\t\t%10d\n"
		"N_ATOMI_INIT: \t\t\t%10d\n"
		"N_ATOMI_MAX: \t\t\t%10d\n" 
		"MIN_N_ATOMICO: \t\t\t%10d\n"
		"SIM_DURATION: \t\t\t%10d\n"
		"STEP: \t\t\t\t%10d\n"
		"N_NUOVI_ATOMI: \t\t\t%10d\n"
		"ENERGY_EXPLODE_THRESHOLD: \t%10d\n"
		"==========================================\n",
		s.energy_demand,
		s.n_atom_init,
		s.n_atom_max,
		s.min_n_atomico,
		s.sim_duration,
		s.step,
		s.n_nuovi_atomi,
		s.energy_explode_threshold
	);
}

settings_info readSettings(char* path){
	int flag = 0; 
	//default path of setting's file
	settings_info sett;
	configFile = fopen(path, "r");
	//token myToken;
	setToken(ERROR,"");
	peek=' ';
	linea=1;
	move();
	start();
	fclose(configFile);
	sett = si;

	verifySettings(sett);
	
	return sett;
}

void verifySettings(settings_info sett){
	int check = isNegORLong(sett.step);
	if (check < 0){
		dprintf(1, "Step e' superiore a 9 cifre\n");
		exit(EXIT_FAILURE);
	}else if (check == 1){
		dprintf(1, "Step e' negativo\n");
		exit(EXIT_FAILURE);
	}

	check = isNegORLong(sett.energy_demand);
	if (check == 1){
		dprintf(1, "Energy Demand e' negativo\n");
		exit(EXIT_FAILURE);
	}
	
	check = isNegORLong(sett.energy_explode_threshold);
	if (check == 1){
		dprintf(1, "Energy Explode e' negativo\n");
		exit(EXIT_FAILURE);
	}

	check = isNegORLong(sett.min_n_atomico);
	if (check == 1){
		dprintf(1, "Min_N_Atomico e' negativo\n");
		exit(EXIT_FAILURE);
	}

	check = isNegORLong(sett.n_atom_init);
	if (check == 1){
		dprintf(1, "N_Atom_Init e' negativo\n");
		exit(EXIT_FAILURE);
	}
	
	check = isNegORLong(sett.n_atom_max);
	if (check == 1){
		dprintf(1, "N_Atom_Max e' negativo\n");
		exit(EXIT_FAILURE);
	}

	check = isNegORLong(sett.n_nuovi_atomi);
	if (check == 1){
		dprintf(1, "N_Nuovi_Atomi e' negativo\n");
		exit(EXIT_FAILURE);
	}

	check = isNegORLong(sett.sim_duration);
	if (check == 1){
		dprintf(1, "Sim_Duration e' negativo\n");
		exit(EXIT_FAILURE);
	}
}

int isNegORLong(int sett){
	int flag = 0;
	if (sett >= 1000000000){
		flag = -1;
	}else if (sett < 0){
		flag = 1;
	}
	return flag;
} 

void move(){
	getToken();

	//printf("token = %d\n",tok.tag);
}

void match(tokens_tags tt){
	if(tok.tag==tt){
		if (tok.tag!= END){
			move();
		}
	}else{
		dprintf(1,"[ERRORE LETTURA IMPOSTAZIONI]linea %d:\tExpected another symbol\n",linea);
		exit(EXIT_FAILURE);
	}
}

void start(){
	if(tok.tag==ENERGY_DEMAND ||tok.tag==N_ATOMI_INIT ||tok.tag==N_ATOMI_MAX ||tok.tag==MIN_N_ATOMICO ||tok.tag==SIM_DURATION ||tok.tag==STEP || tok.tag==N_NUOVI_ATOMI ||tok.tag==ENERGY_EXPLODE_THRESHOLD){	
		statlist();
		match(END);
	}else{
		fprintf(stderr,"[ERRORE LETTURA IMPOSTAZIONI]linea %d:\tIl simbolo %s non e riconosciuto\n",linea,tok.lexemme);
		exit(EXIT_FAILURE);
	}
}

void statlist(){
	if(tok.tag==ENERGY_DEMAND ||tok.tag==N_ATOMI_INIT ||tok.tag==N_ATOMI_MAX ||tok.tag==MIN_N_ATOMICO ||tok.tag==SIM_DURATION ||tok.tag==STEP || tok.tag==N_NUOVI_ATOMI||tok.tag==ENERGY_EXPLODE_THRESHOLD){
		stat();
		statlistp();
	}else{
		fprintf(stderr,"[ERRORE LETTURA IMPOSTAZIONI]linea %d:\tIl simbolo %s non e riconosciuto\n",linea,tok.lexemme);
		exit(EXIT_FAILURE);
	}
	

}

void statlistp(){
	if(tok.tag==ENERGY_DEMAND ||tok.tag==N_ATOMI_INIT ||tok.tag==N_ATOMI_MAX ||tok.tag==MIN_N_ATOMICO ||tok.tag==SIM_DURATION ||tok.tag==STEP || tok.tag==N_NUOVI_ATOMI||tok.tag==ENERGY_EXPLODE_THRESHOLD){
		stat();
		statlistp();
	}else if(tok.tag!=END){
		fprintf(stderr,"[ERRORE LETTURA IMPOSTAZIONI]linea %d:\tIl simbolo %s non e riconosciuto\n",linea,tok.lexemme);
		exit(EXIT_FAILURE);
	}
}

void stat(){
	int n;
	switch (tok.tag){
	case ENERGY_DEMAND:	
		match(ENERGY_DEMAND);
		match(EQ);
		n=atoi(tok.lexemme);
		si.energy_demand=n;
		match(NUM);
		
		match(SMC);
		break;
	case N_ATOMI_INIT:	
		match(N_ATOMI_INIT);
		match(EQ);
		n=atoi(tok.lexemme);
		si.n_atom_init=n;
		match(NUM);
		
		match(SMC);
		break;
	case N_ATOMI_MAX:
		match(N_ATOMI_MAX);
		match(EQ);
		n=atoi(tok.lexemme);
		si.n_atom_max=n;
		match(NUM);
		
		match(SMC);
		break;
	case MIN_N_ATOMICO:
		match(MIN_N_ATOMICO);
		match(EQ);
		n=atoi(tok.lexemme);
		si.min_n_atomico=n;
		match(NUM);
		
		match(SMC);
		break;

	case SIM_DURATION:
		match(SIM_DURATION);
		match(EQ);
		n=atoi(tok.lexemme);
		si.sim_duration=n;
		match(NUM);
		
		match(SMC);
		break;
	case STEP:			
		match(STEP);
		match(EQ);
		n=atoi(tok.lexemme);
		si.step=n;
		match(NUM);
		
		match(SMC);
		break;
	case N_NUOVI_ATOMI:	
		match(N_NUOVI_ATOMI);
		match(EQ);
		n=atoi(tok.lexemme);
		si.n_nuovi_atomi=n;
		match(NUM);
		
		match(SMC);
		break;
		case ENERGY_EXPLODE_THRESHOLD:
		match(ENERGY_EXPLODE_THRESHOLD);
		match(EQ);
		n=atoi(tok.lexemme);
		si.energy_explode_threshold=n;
		match(NUM);
		
		match(SMC);
		break;
	
	default:
		
		fprintf(stderr,"Il simbolo %s non e riconosciuto\n",tok.lexemme);
		break;
	}
}

void setToken(tokens_tags tt,char *lexemma){
	tok.tag= tt;
	strcpy(tok.lexemme,lexemma);
}

char readch(){
	char ch=(char) fgetc(configFile);
	//printf("Ho letto %c\n",ch);
	return ch;
}

void getToken(){
	while (peek == ' ' || peek == '\t' || peek == '\n'  || peek == '\r'){
		if(peek=='\n'){
			linea++;
		}
		peek=readch();
	}
	switch(peek){
		case '#'://E un commento
			while(peek != '\n'&& peek!=(char)-1){
				peek=readch();
			}
			/*if(peek=='\n'){
				linea++;
			}*/
			getToken();
			break; 
		case '=':
			setToken(EQ,"=");
			peek=' ';
			break;
		case ';':
			setToken(SMC,";");
			peek=' ';
			break;
		case (char)-1:
			setToken(END,"");
			break;
		default:
			if(peek=='_' || isdigit(peek) || isalpha(peek)){
				char lexemma[MAX_LENGTH]="";
				char *ptr ;
				ptr= lexemma;
				while(peek=='_' || isdigit(peek) || isalpha(peek)){
					*ptr=peek;
					ptr++;
					peek=readch();
				}
				*ptr='\0';
				//printf("La mia stringa e %s",lexemma);
				if(isNumber(lexemma)>0){
					//printf("Entro qui\n");
					setToken(NUM,lexemma);
					break;
				}else{
					if(strcmp(lexemma,"ENERGY_DEMAND")==0){
						
						setToken(ENERGY_DEMAND,lexemma);
						break;
					}else if(strcmp(lexemma,"N_ATOMI_INIT")==0){
						setToken(N_ATOMI_INIT,lexemma);
						break;
					}else if(strcmp(lexemma,"N_ATOMI_MAX\0")==0){
						setToken(N_ATOMI_MAX,lexemma);
						break;
					}else if(strcmp(lexemma,"MIN_N_ATOMICO\0")==0){
						setToken(MIN_N_ATOMICO,lexemma);
						break;
					}else if(strcmp(lexemma,"SIM_DURATION\0")==0){
						setToken(SIM_DURATION,lexemma);
						break;
					}else if(strcmp(lexemma,"STEP\0")==0){
						setToken(STEP,lexemma);
						break;
					}else if(strcmp(lexemma,"N_NUOVI_ATOMI\0")==0){
						setToken(N_NUOVI_ATOMI,lexemma);
						break;
					}else if(strcmp(lexemma,"ENERGY_EXPLODE_THRESHOLD\0")==0){
						setToken(ENERGY_EXPLODE_THRESHOLD,lexemma);
					}
					
					else{
						setToken(ERROR,lexemma);
						break;
					}
				}
				
			break;
		}
	}
}

//***********************************************************
//UTILITY
/*
La funzione restituisce 1 se la stringa inserita e un numero 
Altrimenti restituisce 0
*/
/*
int isNumber(char s[]){
    for (int i = 0; s[i]!= '\0'; i++){
        if(isdigit(s[i]) == 0)
              return 0;
    }
    return 1;
}*/