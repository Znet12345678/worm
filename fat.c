#include "lib.h"
#include "fat.h"
struct CHS* getPartChs(uint8_t drive,uint8_t partnum){
	char *mbr = malloc(512);
	bios_read_chs(mbr,0,0,1,drive);
	struct PartTab *pt = (struct PartTab *)(mbr + 0x1BE + sizeof(*pt)*partnum);
	struct CHS *ret = malloc(sizeof(*ret));
	bzero(ret,sizeof(*ret));
	if(pt->tsectors == 0)
		return 0;
	ret->c = (pt->starting >> 6) & 1024;
	ret->h = pt->startinghead;

	ret->s = (pt->starting) & 0x20;
	return ret;
}
uint8_t getFatType(uint8_t drive){
	for(int i = 0; i < 4;i++){
		struct CHS *chs = getPartChs(drive,i);
		if(!chs)
			continue;
		_puts("Searching ");
		if(drive == 0)
			_puts("FDA...\n");
		if(drive == 1)
			_puts("FDB...\n");
		if(drive == 0x80)
			_puts("HDA...\n");
		if(drive == 0x81)
			_puts("HDB...\n");
		struct BPB *bpr = malloc(sizeof(*bpr));
		uint8_t *mbr = malloc(512);
		uint8_t ret = bios_read_chs(mbr,chs->c,chs->h,chs->s,drive);
		if(ret != 0){
			_puts("Failed to read drive on sector ");
			puti(chs->s);
			_puts(" head ");
			puti(chs->h);
			_puts(" cylinder ");
			puti(chs->c);
			_puts(". Error code:");
			puti(ret);
			_puts("\n");
			panic();
		}
		for(int i = 0; i < 512;i++)
			_putc(mbr[i]);
		memcpy(bpr,mbr,sizeof(*bpr));
		bpr = (struct BPB*)mbr;
		struct EBR_FAT16 *ebr16 = (struct EBR_FAT16 *)(mbr + sizeof(*bpr));
		struct EBR_FAT32 *ebr32 = (struct EBR_FAT32 *)(mbr + sizeof(*bpr));
		unsigned int tsect = (bpr->tsectors == 0) ? bpr->largesectors : bpr->tsectors;	
		unsigned int fat_size = (bpr->sectorsperfat == 0) ? ebr32->sectorsperfat : bpr->sectorsperfat;
		unsigned short rds =  ((bpr->ndent*32)+(bpr->bytes_per_sector-1))/bpr->bytes_per_sector;
		unsigned int ds = bpr->tsectors - (bpr->nrsect + (bpr->tblcount * fat_size) + rds);
		unsigned int tc = ds/bpr->sectors_per_cluster;
		_puts("oi\n");
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
