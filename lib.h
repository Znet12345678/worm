#ifndef __LIB_H
#define __LIB_H
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;
typedef char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long int64_t;
typedef uint64_t size_t;
struct mem{
	uint8_t alloc;
	size_t size;

};
struct CHS{
	uint16_t c;
	uint16_t h;
	uint16_t s;
};
void _putc(char c);
void memcpy(void *dest,void *src,unsigned long n);
void init_mem();
void panic();
void main();
void _puts(const char *str);
unsigned long strlen(const char *str);
uint8_t bios_read_chs(void *pntr,unsigned short c,unsigned short h,unsigned short s,unsigned short d);
void *malloc(size_t size);
void free(void *pntr);
void bzero(void *dest,unsigned long n);
void puti(unsigned int n);
#endif
