#ifndef __FAT_H
#define __FAT_H
struct BPB{
	uint8_t _asm[3];
	uint8_t ident[8];
	uint16_t bytes_per_sector;
	uint8_t sectors_per_cluster;
	uint16_t nrsect;
	uint8_t tblcount;
	uint16_t ndent;
	uint16_t tsectors;
	uint8_t mdt;
	uint16_t sectorsperfat;
	uint16_t sectorspertrack;
	uint16_t theads;
	uint32_t lba_begin;
	uint32_t largesectors;
}__attribute__((packed));
struct PartTab{
	uint8_t boot;
	uint8_t startinghead;
	uint16_t starting;//Sector = (starting >> 6) & 64 | cylinder = starting & 1024
	uint8_t sysid;
	uint8_t endinghead;
	uint16_t ending;//Sector = (ending >> 6) & 64  | cylinder = ending & 1024;
	uint32_t lba;
	uint32_t tsectors;
};
struct EBR_FAT16{
	uint8_t drivenum;
	uint8_t flags;
	uint8_t sig;
	uint32_t serial;
	uint8_t vlbl[11];
	uint8_t sysident[8];
}__attribute__((packed));
struct EBR_FAT32{
	uint32_t sectorsperfat;
	uint16_t flags;
	uint16_t vernum;
	uint32_t rootcluster;
	uint16_t sectorFSInfo;
	uint16_t bupbs;
	uint8_t resv[12];
	uint8_t drivenum;
	uint8_t sig;
	uint32_t serial;
	uint8_t vlbl[11];
	uint8_t sysident[8];
}__attribute__((packed));
struct dirent{
	uint8_t name[11];
	uint8_t attr;
	uint8_t resv;
	uint8_t time;
	uint16_t timec;
	uint16_t date;
	uint16_t adate;
	uint16_t first_clusterh;
	uint8_t lastmod;
	uint8_t lastmodd;
	uint16_t first_clusterl;
	uint32_t size;
};
struct lonkboi{
	uint8_t order;
	uint16_t first5[5];
	uint8_t attr;
	uint8_t enttype;
	uint8_t check;
	uint16_t next6[6];
	uint16_t zero;
	uint16_t final2[2];
};
#define TYPE_FAT12 0
#define TYPE_FAT16 1
#define TYPE_FAT32 2
#define TYPE_ExFat 4
uint8_t getFatType(uint8_t drive);
#endif
