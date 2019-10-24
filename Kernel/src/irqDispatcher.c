// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <time.h>
#include <keyboard.h>
#include <stdint.h>
#include <process.h>

// Hardware handlers functions
static void int_20();
static void int_21();

void (* handlers[]) () = {int_20, int_21};

// Dispatcher for hardware interrupts
void irqDispatcher(uint64_t irq) {
	handlers[irq]();
}

void int_20() {
	timer_handler();
	context_switch();
}

void int_21() {
	keyboard_handler();
}