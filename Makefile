all:
	nasm -f bin pre.asm -o pre.bin
	nasm -f bin stage1.asm -o stage1.bin
	nasm -f bin padding.asm -o padding.bin
	nasm -f elf stage2.asm -o stage2.o
	nasm -f elf lib.asm -o lib.o
	i386-elf-gcc -nostdlib -ffreestanding -m16 -c string.c -o string.o
	i386-elf-gcc -nostdlib -ffreestanding -m16 -c main.c -o main.o
	i386-elf-gcc -nostdlib -ffreestanding -m16 -c puts.c -o puts.o
	i386-elf-gcc -nostdlib -ffreestanding -m16 -c lib.c -o lib.o
	i386-elf-gcc -nostdlib -ffreestanding -m16 -c fat.c -o fat.o
	i386-elf-gcc -Tlinker.ld -m16 -nostdlib puts.o fat.o lib.o main.o string.o stage2.o -o stage2.elf
	cat pre.bin stage1.bin stage2.elf padding.bin > bootloader.img.1
	dd if=/dev/zero of=zero.img bs=1M count=100
	cat bootloader.img.1 zero.img > bootloader.img
	gcc pad.c -o pad
	./pad bootloader.img
