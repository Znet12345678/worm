[bits 16]
[org 0x1000]
sig:db 'WORM'
jmp _start
jmpelf:db 'Jumping to elf...',0
ldelf:db 'Loading elf...',0
returnedmsg:db 'Returned to stage1! This should not happen!',0
errmsg:db 'Error!',0
puts_bios:
mov ah,0x0e
loop:
mov al,[bx]
int 0x10
inc bx
cmp byte [bx],0
je r
jmp loop
r:ret
zerrm:db 'Negative size fault!',0
msg: db 'Parsing elf...',0
e_hdr:
e_ident:times 0x10 db 0
e_type:dw 0
e_machine:dw 0
e_version: dd 0
e_entry:dd 0
e_phoff:dd 0
e_shoff:dd 0
e_flags:dd 0
e_ehsize:dw 0
e_phdrsize:dw 0
e_phnum:dw 0
e_shentsize:dw 0
e_shnum:dw 0
e_shstrndx: dw 0
e_end:
inc:dw 0
memcpy:
cmp dx,0
jl zerr
mov word [inc],0
jmp g
srcpntr:dw 0
currb:db 0
count:dw 0
g:mov word [srcpntr],ax
mov word [count],dx
l:
mov ax,[count]
cmp [inc],ax
jge r2
mov bx,cx
mov bx,[bx]
mov byte [currb],bl
mov word bx,[srcpntr]
mov al,[currb]
mov byte [bx],al
inc word [srcpntr]
inc cx
inc word [inc]
jmp l
r2:ret
phdr:
p_type:dd 0
p_offset:dd 0
p_vaddr:dd 0
p_paddr:dd 0
p_filesz:dd 0
p_memsz:dd 0
p_flags:dd 0
p_align:dd 0
phdr_end:
cntr:dd 0
_start:
mov ax,ds
mov es,ax
mov ah,0x02
mov al,30
mov cl,3
mov ch,0
mov dh,0
mov bx,0x2000
mov dl,[0xFFFF]
int 0x13
cmp ah,0
jne err
mov bx,msg
call puts_bios
mov ax,e_hdr
mov cx,0x2000
mov dx,e_end-e_hdr
call memcpy
mov bx,ldelf
call puts_bios
cmp byte [0x2000],0x7f
jne err
cmp byte [0x2001],'E'
jne err
cmp byte [0x2002],'L'
jne err
cmp byte [0x2003],'F'
jne err
mov word [cntr],0
loop2:
mov ax,[cntr]
push dx
mul word [e_phdrsize]
pop dx
add ax,0x2000
add ax,[e_phoff]
mov cx,ax
mov ax,phdr
call memcpy
mov cx,[p_offset]
add cx,0x2000
mov ax,[p_vaddr]
mov dx,[p_filesz]

call memcpy
mov ax,[e_phnum]
inc word [cntr]
cmp [cntr],ax
jge load_elf
jmp loop2
load_elf:
mov bx,jmpelf
call puts_bios

jmp [e_entry]
jmp returned
err:mov bx,errmsg
call puts_bios
hng:jmp hng 
zerr:
mov bx,zerrm
call puts_bios
jmp hng
returned:mov bx,returnedmsg
call puts_bios
jmp hng
times 512 - ($ - $$) db 0
