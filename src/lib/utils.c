#include "util.h"
#include <ctype.h>
#include <string.h>


int isNumber(char s[]){
    for (int i = 0; s[i]!= '\0'; i++){
        if(isdigit(s[i]) == 0)
              return 0;
    }
    return 1;
}