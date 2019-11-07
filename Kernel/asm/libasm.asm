GLOBAL cpu_vendor
GLOBAL write_port
GLOBAL read_port
GLOBAL set_stack
GLOBAL sem_up
GLOBAL sem_down
GLOBAL int_20


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

	;Asigno en el stack pointer el parametro que recibi (osea el stack pointer del proceso actual)
	mov rsp, rdi
	push 0x000 ; SS ; IRET
	push rdi
	push 0x202 ; RFLAGS
	push 0x8 ; CS
	push rsi ; rip   
	push 0x0 ; pushaq registers
	push rdi
	push rdi
	push rdi
	push rdi
	push rdi
	push rdi
	push rdi
	push rdi
	push rdi
	push rdi
	push rdi
	push rdi
	push rdi
	push rdi
	mov rax, rsp

	mov rsp, rbp
	pop rbp
	ret

; sem_up

sem_up:
push rbp
	mov rbp, rsp
	push rax
	mov rax, [rdi] 
	inc rax
	xchg rax, rdi
	pop rax
	mov rsp, rbp
	pop rbp
	ret

; sem_down, recibe en RDI la dir del contador a restar

sem_down:
	push rbp
	mov rbp, rsp
	push rax

	mov rax, [rdi] 
	dec rax
	xchg rax, rdi
	
	pop rax
	mov rsp, rbp
	pop rbp
	ret

int_20:
	push rbp
	mov rbp, rsp

	int_20

	mov rsp, rbp
	pop rbp
	ret