#include "lib.h"
#include "fat.h"
struct CHS* getPartChs(uint8_t drive,uint8_t partnum){
	char *mbr = malloc(512);
	bios_read_chs(mbr,0,0,1,drive);
	struct PartTab *pt = mbr + 0x1BE + sizeof(*bpb)*partnum;
	if(pt->boot == 0)
		return 0;
	struct CHS *ret = malloc(sizeof(*ret));
	ret->c = pt->starting >> 6;
	ret->h = pt->startinghead;
	ret->s = pt->starting & 64;
	free(mbr);
	return ret;
}
uint8_t getFatType(uint8_t drive){
	for(int i = 0; i < 4;i++){
		struct CHS *chs = getPartChs(drive,i);
		if(!chs)
			continue;
		struct BPR *bpr;
		uint8_t *mbr = malloc(512);
		bios_read_chs(mbr,chs->c,chs->h,chs->s,drive);
		bpr = (struct BPR*)mbr;

	}
}
