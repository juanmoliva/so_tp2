// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <syscalls.h>
#include <stdint.h>

#include <console.h>

#define SYSCALL_COUNT	25

// Software handlers functions
static uint64_t syscall_00 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_01 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_02 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_03 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_04 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_05 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_06 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_07 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_08 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
/////////////////////////////////////////////////////////////////////
static uint64_t syscall_09 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_10 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_11 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_12 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_13 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_14 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_15 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_16 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_17 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_18 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_19 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_20 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_21 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_22 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_23 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_24 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_25 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_26 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_27 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_28 (uint64_t rdi, uint64_t rsi, uint64_t rdx);
static uint64_t syscall_29 (uint64_t rdi, uint64_t rsi, uint64_t rdx);



extern void hang(); // Ubicada en loader.asm

uint64_t (* syscalls[]) (uint64_t rdi, uint64_t rsi, uint64_t rdx) = {syscall_00, syscall_01, syscall_02, syscall_03, 
																	syscall_04, syscall_05, syscall_06, syscall_07, 
																	syscall_08, syscall_09, syscall_10, syscall_11, 
																	syscall_12, syscall_13, syscall_14, syscall_15,
																	syscall_16, syscall_17, syscall_18, syscall_19,
																	syscall_20, syscall_21, syscall_22, syscall_23,
																	syscall_24, syscall_25, syscall_26};

// Dispatcher for software interrupts
uint64_t handleSyscall(uint64_t sirq, uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	if (sirq <= SYSCALL_COUNT)
		return syscalls[sirq](rdi, rsi, rdx);
	return 1;
}

uint64_t syscall_00 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	read_handler(rdi, (char *)rsi, rdx);
	return 0;
}

uint64_t syscall_01 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	write_handler(rdi, (char *)rsi, rdx);
	return 0;
}

uint64_t syscall_02 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	return time_handler();
}

uint64_t syscall_03 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	clear_handler();
	return 0;
}

uint64_t syscall_04 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	return rtc_handler((char) rdi);
}

uint64_t syscall_05 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	sleep_handler(rdi);
	return 0;
}

uint64_t syscall_06 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	beep_handler(rdi, rsi);
	return 0;
}

uint64_t syscall_07 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	hang();
	return 0;
}

uint64_t syscall_08 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	pixel_handler(rdi, rsi, rdx);
	return 0;
}

uint64_t syscall_09 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	/* si 'rdi' es cero: va a asignar 'rsi' bytes en memoria y devolver la direccion en la que los asignó.
	   si no: va a liberar los bloques alocados en la direccion 'rsi' y devolver 0 si pudo hacerlo. */
	return (uint64_t) memory_handler( (char) rdi, rsi);
}

uint64_t syscall_10 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	/* dependiendo del valor de rdi, devuelve:
		rdi == 0 --> memoria total
		rdi != 0 --> memoria disponible
	*/
	return memory_state_handler( (char) rdi);
}

uint64_t syscall_11 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	/* creates a new process, returns the new process id.
	*/
	//rsi ----> tiene la priority
	//rdi ----> tiene el rip
	return create_process_handler((void *) rdi,(int) rsi, (char *) rdx);
}
uint64_t syscall_12 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	/* UPDATE PROCESS PRIORITY
	*/
	//rsi ----> tiene la priority
	//rdi --> pid
	return update_process_priority_handler((int) rdi, (int) rsi);
}
uint64_t syscall_13 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	/* UPDATE PROCESS STATE
	*/
	//rsi ----> tiene el nuevo state
	//rdi --> pid
	return update_process_state_handler((int) rdi, (char) rsi);
}
uint64_t syscall_14 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	/* LIST PROCESSES 
	*/

	return (uint64_t) list_processes_handler();
}

uint64_t syscall_15 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	/* LIST SEM 
	*/

	return list_sem_handler();
}

uint64_t syscall_16 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	/* List Pipes
	*/

	return list_pipes_handler();
}

uint64_t syscall_17 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	/* Get calling PID
	*/

	return get_pid_handler();
}

uint64_t syscall_18 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	/* kill process with rdi PID.
	*/

	return kill_process_handler((int) rdi);
}



///SEMAFOROS//////////////////////////////////////////////////
//Puse los identifiers con INT porque sino dps me va a quedar el PID de los procesos y el PID de los semaforos en dos tipos distintos
uint64_t syscall_19 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	/* Init Sem with rdi identifier, rsi initial count
	*/

	//return init_sem_handler((char *) rdi, (int) rsi);
	return init_sem_handler((int) rdi, (int) rsi);
}

uint64_t syscall_20 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	/* open sem with rdi identifier
	*/

	//return open_sem_handler((char *) rdi);
	return open_sem_handler((int) rdi);
}

uint64_t syscall_21 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	/* close sem with rdi identifier.
	*/

	//return close_sem_handler((char *) rdi);
	return close_sem_handler((int) rdi);
}

uint64_t syscall_22 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	/* sem_wait with rdi identifier.
	*/

	//return sem_wait_handler((char *) rdi);
	return sem_wait_handler((int) rdi);
}

uint64_t syscall_23 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	/* sem_post with rdi identifier.
	*/

	//return sem_post_handler((char *) rdi);
	return sem_post_handler((int) rdi);
}

////////////////////////////////////////////////////////





uint64_t syscall_24 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	/* create pipe with rdi identifier.
	*/

	return create_pipe_handler((char *) rdi);
}

uint64_t syscall_25 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	/* write rsi content on pipe with rdi identifier.
	*/

	return write_pipe_handler((char *) rdi, (char *) rsi);
}

uint64_t syscall_26 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	/* read content on pipe with rdi identifier and store it on rsi string
	*/

	return read_pipe_handler((char *) rdi, (char *) rsi);
}

uint64_t syscall_27 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	/* write rsi content on pipe with rdi identifier.
	*/

	return block_process_handler((char *) rdi, (char *) rsi);
}
uint64_t syscall_28 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	/* write rsi content on pipe with rdi identifier.
	*/

	return nice_handler((int) rdi, (int) rsi, );
}

uint64_t syscall_29 (uint64_t rdi, uint64_t rsi, uint64_t rdx) {
	/* write rsi content on pipe with rdi identifier.
	*/

	return loop_handler();
	
}
//ACA AGREGAR TODO LO Q QUIERAS Q TENGA CONTACTO CON EL USERLAND!!!!!! 
//PORQUE ESTO ES LO QUE LLAMA EL CHABON!!!!!!! DPS DE ESTO SE PASA A SYSCALLS.c Y DPS A PROCESS.C
