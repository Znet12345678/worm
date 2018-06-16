#include "lib.h"
void _putc(char c){
	if(c == '\n')
		return;
	__asm__("mov $0x0e,%ah");
	__asm__("mov %0,%%al" : :"m"(c));
	__asm__("int $0x10");
}

void wipe(){
	__asm__("mov $0,%ah");
	__asm__("mov $3,%al");
	__asm__("int $0x10");
	_puts("Wiped screen\n");
}
void _puts(const char *pntr){
	int i = 0;
	while(i < strlen(pntr)){
		if(pntr[i] == '\n'){
			__asm__("mov $0x03,%ah");
			__asm__("xor %bx,%bx");
			__asm__("mov $0,%bh");
			__asm__("int $0x10");
			__asm__("mov $0x02,%ah");
			__asm__("mov $0,%dl");
			__asm__("inc %dh");
			int lines;
			__asm__("mov %%dh,%0" : "=m"(lines));
			__asm__("int $0x10");
//			if(lines > 24)
//				wipe();
			i++;
			continue;
		}
		_putc(pntr[i]);
		i++;
	}
}
