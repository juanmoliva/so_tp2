// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <exceptions.h>
#include <console.h>
#include <interrupts.h>
#include <time.h>	// Para usar la funcion de sleep

static void zero_division();
static void invalid_opcode();
static void not_imp();

RegisterSet regState;
ExceptionStackFrame excState;

extern void goToUserland();

void (* excHandlers[]) () = {zero_division, not_imp, not_imp, not_imp, not_imp, not_imp, invalid_opcode};

void exceptionDispatcher(int exc) {
	_cli();
	
	// Algunas excepciones cargan en el stack un codigo de error
	// Segun carguen o no, accedo al stack en busca el Instruction Pointer

	//Estas excepciones no tienen Error Code
	if (exc == 8 || (exc >= 10 && exc <= 14) || exc == 17)
        __asm__ volatile ("mov 16(%%rsp), %0;" : "=a" (excState.rip));
	else
	    __asm__ volatile ("mov 24(%%rsp), %0;" : "=a" (excState.rip));

	getRegisters(); 	// Obtengo los valores actuales de los registros
	
	clear_console();	// Limpio la consola
	print_char('\n');
	excHandlers[exc]();	// Ejecuto la rutina de atencion de la excepcion
	// Imprimo el estado de los registros
	printError("RIP: %d\n", excState.rip);
    printError("RAX: %d\nRBX: %d\nRCX: %d\nRDX: %d\nRDI: %d\nRSI: %d\nRBP: %d\nRSP: %d\nR8: %d\nR9: %d\n", 
		regState.rax, regState.rbx, regState.rcx, regState.rdx, regState.rdi, regState.rsi, regState.rbp, regState.rsp, regState.r8, regState.r9);
	print("\n\nRebooting to userland");

    _sti();
    sleep(2000);
	goToUserland(); 	// Vuelvo a userland
}

void getRegisters(){
	__asm__ volatile ( "mov %%rax, %0;" : "=a" (regState.rax));
    __asm__ volatile ( "mov %%rbx, %0;" : "=a" (regState.rbx));
    __asm__ volatile ( "mov %%rcx, %0;" : "=a" (regState.rcx));
    __asm__ volatile ( "mov %%rdx, %0;" : "=a" (regState.rdx));
    __asm__ volatile ( "mov %%rdi, %0;" : "=a" (regState.rdi));
    __asm__ volatile ( "mov %%rsi, %0;" : "=a" (regState.rsi));
    __asm__ volatile ( "mov %%rbp, %0;" : "=a" (regState.rbp));
    __asm__ volatile ( "mov %%rsp, %0;" : "=a" (regState.rsp));
    __asm__ volatile ( "mov %%r8, %0;"  : "=a" (regState.r8));
    __asm__ volatile ( "mov %%r9, %0;"  : "=a" (regState.r9));
}

static void zero_division() {
    printError("Exception 0: Division By Zero\n");
}

static void invalid_opcode() {
    printError("Exception 6: Invalid Opcode\n");
}

static void not_imp() {
    printError("Exception not implemented\n");
}