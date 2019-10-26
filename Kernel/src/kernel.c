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

int main() {
    // ncClear();
    goToUserland();
	// ncNewline();

	// int i = 0;
	// char car;
    // while (i < 10) {
    //     if ((car = read_character()) != 0){
	// 		ncPrintChar(car);
	// 		i++;
	// 	}
    // }
	return 0;
}