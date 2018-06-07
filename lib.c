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
	__asm__("mov %0,%%bx" : "=m"(pntr));
	__asm__("int $0x13");
	int ret;
	__asm__("mov %%ah,%0" : "=m"(ret));
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
char *tostr(int n){
	char *str = malloc(80);
	int indx = intlen(n)-1;
	while(n > 0){
		str[indx] = (n % 10) + '0';
		indx--;
		n/=10;
	}
	return str;
}
void *malloc(size_t n){
	struct mem *m = (struct mem*)0xF000;
	while(m->alloc){

		m+=m->size;
	}
	int allocated = 0;
	while(allocated < n){
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
