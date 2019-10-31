GLOBAL cpu_vendor
GLOBAL write_port
GLOBAL read_port
GLOBAL set_stack


section .text

; void write_port(char port, char reg)
write_port:
	mov al, sil
	mov dx, di
	out dx, al
	ret

; char read_port(char port)
read_port:
	mov dx, di
	in al, dx
	ret


cpu_vendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

; set_stack(unsigned long addr)
set_stack:
	push rbp
	mov rbp, rsp
	mov rsp, rdi
	push 0x000 ; SS
	push rdi
	push 0x202 ; RFLAGS
	push 0x8 ; CS
	push rsi ; rip
	push 0x0 ; pushaq registers
	push 0x0
	push 0x0
	push 0x0
	push 0x0
	push 0x0
	push 0x0
	push 0x0
	mov rsp, rbp
	pop rbp
	ret

