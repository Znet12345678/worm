#include "lib.h"
#include "fat.h"
struct CHS* getPartChs(uint8_t drive,uint8_t partnum){
	char *mbr = malloc(512);
	bios_read_chs(mbr,0,0,1,drive);
	struct PartTab *pt = (struct PartTab *)(mbr + 0x1BE + sizeof(*pt)*partnum);
	struct CHS *ret = malloc(sizeof(*ret));
	if(pt->tsectors == 0)
		return 0;
	ret->c = pt->starting >> 6;
	ret->h = pt->startinghead;
	ret->s = pt->starting & 64;
	return ret;
}
uint8_t getFatType(uint8_t drive){
	for(int i = 0; i < 4;i++){
		struct CHS *chs = getPartChs(drive,i);
		if(!chs)
			continue;
		struct BPB *bpr;
		uint8_t *mbr = malloc(512);
		bios_read_chs(mbr,chs->c,chs->h,chs->s,drive);
		bpr = (struct BPB*)mbr;
		struct EBR_FAT16 *ebr16 = (struct EBR_FAT16 *)(mbr + sizeof(*bpr));
		struct EBR_FAT32 *ebr32 = (struct EBR_FAT32 *)(mbr + sizeof(*bpr));	
		unsigned int tsect = (bpr->tsectors == 0) ? bpr->largesectors : bpr->tsectors;	
		unsigned int fat_size = (bpr->sectorsperfat == 0) ? ebr32->sectorsperfat : bpr->sectorsperfat;
		unsigned int rds = ((bpr->ndent*32)+(bpr->bytes_per_sector-1))/bpr->bytes_per_sector;
		unsigned int ds = tsect - (bpr->nrsect + (bpr->tblcount * fat_size)) + rds;
		unsigned int tc = ds/bpr->sectors_per_cluster;
		if(tc < 4085)
			return 0;
		if(tc < 65525)
			return 1;
		if(tc < 268435445)
			return 2;
		return 3;
	}
	return 4;
}
