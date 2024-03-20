//#include "util.h"

//Max of number preferences
#define MAX_PREFERENCES 7
#define MAX_LENGTH 128


typedef enum{
	EQ = (int)'=',
	SMC = (int)';',
	END = -1, 
	ERROR=-2,

	NUM=256,
	ENERGY_DEMAND=257,
	N_ATOMI_INIT=258,
	N_ATOMI_MAX=259,
	MIN_N_ATOMICO=260,

	SIM_DURATION=261,

	STEP=262,
	N_NUOVI_ATOMI=263,
	ENERGY_EXPLODE_THRESHOLD=264
}tokens_tags;

typedef struct {
	int energy_demand;
	int n_atom_init;
	int n_atom_max;
	int min_n_atomico;

	int sim_duration;

	int step;
	int n_nuovi_atomi;
	int energy_explode_threshold;

}settings_info;

typedef struct{
	tokens_tags tag;
	char lexemme[MAX_LENGTH];
}token;


void make_default_settings_file();
int verify_file(char *path);
void printSettings(settings_info s);
settings_info readSettings(char* path);
void verifySettings(settings_info sett);
int isNegORLong(int sett);
void move();
void match(tokens_tags tt);
void start();
void statlist();
void statlistp();
void stat();
void setToken(tokens_tags tt,char *lexemma);
char readch();
void getToken();
int isNumber(char s[]);
