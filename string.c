#include "lib.h"
unsigned long strlen(const char *str){
	unsigned long indx = 0;
	while(str[indx] != 0)
		indx++;
	return indx;
}
