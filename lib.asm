[bits 16]
global puts
puts:
xor ax,ax
xor bx,bx
xor cx,cx
xor dx,dx
pop bx

loop:
cmp byte [bx],0
je r
cmp byte [bx],0xa
je nl
mov ah,0x0e
mov al,[bx]
int 0x10
inc bx
jmp loop
nl:mov ah,0x03
int 0x10
mov ah,0x02
inc dh
mov dl,0
push bx
xor bx,bx
mov bh,0
int 0x10
pop bx
inc bx
jmp loop
r:ret
