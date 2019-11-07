// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <strings.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <idtLoader.h>
#include <defs.h>
#include <videoDriver.h>
#include <console.h>

#include <interrupts.h>
#include <memory.h>
#include <process.h>
//#include <time.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();

void goToUserland(){
	((EntryPoint)sampleCodeModuleAddress)();
}

void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	char buffer[10];

	cpu_vendor(buffer);

	// Llena con los addresses a donde copia los modulos
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

	//Inicializamos el Memory Manager
	init_mm();
	//Inicializamos toda la parte de procesos
	//init_process_list(); Ta todo en el init scheduler alfinal
	init_scheduler();
	////////////////////////////////////////
	initVideoDriver();
  	init_console();
	

	load_idt();

	return getStackBase();
}

// int main() {
// 	clear();
// 	// Load IDT table
// 	load_idt();	

// 	// Execute userland code
// 	((EntryPoint)sampleCodeModuleAddress)();

// 	return 0;
// }
void init_code() {
	//Esto crea otro proceso que es el SHELL
	create_process(10,sampleCodeModuleAddress, "shell");
}

int main() {
	//Creamos primer proceso! Proceso Padre.
    create_process(0,&init_code, "init");
	//Corremos init_code osea la funcion del primer proceso
	init_code();

	//La funcion HLT espera una interrupcion de hardware (un timer tick) 
	_hlt();
	
	return 0;
}