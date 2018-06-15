#include "lib.h"
uint8_t bios_read_chs(void *pntr,unsigned short c,unsigned short h,unsigned short s,unsigned short d){
	__asm__("mov %ds,%ax");
	__asm__("mov %ax,%es");
	__asm__("xor %ax,%ax");
	__asm__("mov $0x02,%ah");
	__asm__("mov $0x01,%al");
	__asm__("mov %0,%%ch" : :"m"(c));
	__asm__("mov %0,%%cl" : :"m"(s));
	__asm__("mov %0,%%dh" : :"m"(h));
	__asm__("mov %0,%%dl" : :"m"(d));
	__asm__("mov %0,%%bx" : : "m"(pntr));
	__asm__("int $0x13");
	int ret;
	__asm__("mov %%ah,%0" :  : "m"(ret));
	return ret;
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
		if(!m->alloc)
			m+=sizeof(*m);
		int sv = m->size;
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
