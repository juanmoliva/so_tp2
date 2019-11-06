
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt
GLOBAL halt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler

GLOBAL _sirqHandler

GLOBAL _exception00Handler
GLOBAL _exception06Handler

EXTERN irqDispatcher
EXTERN handleSyscall
EXTERN exceptionDispatcher
EXTERN schedule

SECTION .text

%macro pushStateSome 0
	push rbx
	push rcx
	push rbp
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popStateSome 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rbp
	pop rcx
	pop rbx
%endmacro

%macro pushStateAll 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popStateAll 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro irqHandlerMaster 1
	pushStateAll

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popStateAll
	iretq
%endmacro

%macro sirqHandlerMaster 0
	pushStateSome	; Todos los registros menos los utilizados por syscalls
	call sirqDispatcher
	popStateSome
	iretq
%endmacro

%macro exceptionHandler 1
	pushStateAll

	mov rdi, %1 ; pasaje de parametro
	call exceptionDispatcher

	popStateAll
	iretq
%endmacro


_hlt:
	sti
	hlt
	ret

halt:
	hlt
	ret

_cli:
	cli
	ret

_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	pushStateAll
	mov rdi, 0 ; pasaje de parametro
	call irqDispatcher
	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al
	popStateAll
	pushStateAll
	mov rdi, rsp
	call schedule
	mov rsp, rax
	popStateAll
	iretq

;Keyboard
_irq01Handler:
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5

;Syscall
_sirqHandler:
	call handleSyscall
	iretq

;Zero Division Exception
_exception00Handler:
	exceptionHandler 0

;Invalid Opcode Exception
_exception06Handler:
	exceptionHandler 6

haltcpu:
	cli
	hlt
	ret



SECTION .bss
	aux resq 1