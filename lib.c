#include "lib.h"
void resetdisk(){
	__asm__("mov $0,%ah");
	__asm__("int $0x13");
}
struct CHS *getdp(uint8_t drive){
#ifdef DEBUG
	_puts("getdp( ... )\n");
#endif
	resetdisk();
	struct CHS *ret = malloc(sizeof(*ret));
	__asm__("mov $0x08,%ah");
	__asm__("mov %0,%%dl" : :"m"(drive));
	__asm__("int $0x13");
	uint8_t head,c,s,retc;
	__asm__("mov %%dh,%0" : "=m"(head));
	__asm__("mov %%ch,%0" : "=m"(c));
	__asm__("mov %%cl,%0" : "=m"(s));
	ret->h = head;
	ret->s = s;
	ret->c = c;
	resetdisk();
	__asm__("mov $2,%ah");
	__asm__("mov $1,%al");
	__asm__("mov %0,%%dl" : :"m"(drive));
	__asm__("mov $0,%dh");
	__asm__("mov $1,%cx");
	__asm__("int $0x13");
	__asm__("mov %%ah,%0" : "=m"(retc));
	resetdisk();
	if(retc != 0)
		return 0;
	return ret;	
}
struct CHS *convCHS(struct CHS *drive,struct CHS *chs){
#ifdef DEBUG
	_puts("convCHS( ... )\n");
#endif
	struct CHS *ret = malloc(sizeof(*ret));
	ret->c = drive->c == 0? :chs->c % drive->c;
	ret->h = drive->h == 0 ? : chs->h % drive->h;
	uint16_t val;
	val = chs->c;
	while(val > drive->c){
		ret->h++;
		val--;
	}
	val = chs->h;
	while(val > drive->h){
		ret->s++;
		val--;
	}
	return ret;
}
uint8_t bios_read_chs(void *pntr,unsigned short c,unsigned short h,unsigned short s,unsigned short d){
#ifdef DEBUG
	_puts("bios_read_chs( ... )\n");
#endif
	resetdisk();
	struct CHS *drive = getdp(d);
	if(!drive)
		return -1;
	struct CHS *chs = malloc(sizeof(*chs));
	chs->c = c;
	chs->h = h;
	chs->s = s;
	chs = convCHS(drive,chs);
	if(!chs)
		return 0;
	__asm__("mov %ds,%ax");
	__asm__("mov %ax,%es");
	__asm__("xor %ax,%ax");
	__asm__("mov $0x02,%ah");
	__asm__("mov $0x01,%al");
	__asm__("mov %0,%%ch" : :"m"(chs->c));
	__asm__("mov %0,%%cl" : :"m"(chs->s));
	__asm__("mov %0,%%dh" : :"m"(chs->h));
	__asm__("mov %0,%%dl" : :"m"(d));
	__asm__("mov %0,%%bx" :  "=m"(pntr));
	__asm__("int $0x13");
	resetdisk();
	uint8_t ret;
	__asm__("mov %%ah,%0" :  "=m"(ret));
	return ret;
}
void pdskerr(int err){
	_puts("Disk error ");
	puti(err);
	_puts(" has occured\n");
}
int intlen(int n){
	int ret = 0;
	while(n > 0){
		n/=10;
		ret++;
	}
	return ret;
}
void bzero(void *pntr,uint64_t n){
	for(unsigned long i = 0;i < n;i++)
		*(uint8_t*)pntr = 0;
}
void puti(unsigned int n){
	if(n == 0){
		_puts("0");
		return;
	}
	int8_t indx = intlen(n)-1;
	uint8_t *pntr = malloc(1024);
	bzero(pntr,1024);
	while(indx >= 0){
		pntr[indx] = (uint8_t)(n % 10) + '0';
		indx--;
		n/=10;
	}
	_puts(pntr);
}
void init_mem(){
	
}
void memcpy(void *dest,void *src,unsigned long n){
	for(unsigned long i = 0; i < n;i++)
		*(unsigned char*)(dest + i) = *(unsigned char*)(src + i);
}
void *malloc(size_t n){
	struct mem *m = (struct mem*)0xA000;
	while(m->alloc){
		m+=m->size;
	}
	int allocated = 0;
	while(allocated < n){
		if(m->alloc){
			m+=sizeof(*m);
			continue;
		}int sv = m->size;
		if(m->size > n){
			m->alloc = 1;
			m->size = n;
			allocated+=n;
			struct mem *pntr = m + sizeof(*m)+n;
			pntr->alloc = 0;
			pntr->size=sv-n;
		}else if(m->size == n){
			allocated+=n;	
		}else if(m->size == 0){
			m->size = n;
			allocated+=n;
		}
		else{
			m+=m->size;
		}
	}
	return (void*)(m + sizeof(*m));
}
void free(void *pntr){
	struct mem *m = (struct mem *)(pntr-sizeof(*m));
	m->alloc = 0;
}
void panic(){
	_puts("panic()");
	while(1){ __asm__("hlt"); }
}
