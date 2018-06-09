#include <stdio.h>
int main(int argc,char *argv[]){
	if(argc != 2)
		return -1;
	FILE *f = fopen(argv[1],"r+b");
	fseek(f,0,SEEK_END);
	int size = ftell(f);
	int add = 512 - (size % 512);
	for(int i = 0; i < add;i++)
		putc(0,f);
	fclose(f);
	return 0;
}
