#include "lib.h"
void display(int r){
	if(r == 0){
		_puts("FOUND\n");
	}
}
void main(){
	const char *pntr = "Welcome to worm, a real mode x86 bootloader\n";
	_puts(pntr);
	_puts("->Searching for usable disks...\n");
	_puts("FDA->\n");
	uint8_t *r = malloc(4);
	char *fapntr = malloc(512);
	r[0] = bios_read_chs(fapntr,0,0,1,0);
	display(r[0]);
	_puts("FDB->\n");
	char *fbpntr = malloc(512);
	r[1] = bios_read_chs(fbpntr,0,0,1,1);
	display(r[1]);
	_puts("HDA->\n");
	char *hapntr = malloc(512);
	r[2] = bios_read_chs(hapntr,0,0,1,0x80);
	display(r[2]);
	_puts("HDB->\n");
	char *hbpntr = malloc(512);
	r[3] = bios_read_chs(hbpntr,0,0,1,0x81);
	display(r[3]);
	if(r[0] != 0)
		free(fapntr);
	if(r[1] != 0)
		free(fbpntr);
	if(r[2] != 0)
		free(hapntr);
	if(r[3] != 0)
		free(hbpntr);
	int found = 0;
	for(int i = 0; i < 4 && !found;i++){
		if(r[i] == 0)
			found = 1;
	}
	if(!found){
		panic();
	}
	while(1);
}
