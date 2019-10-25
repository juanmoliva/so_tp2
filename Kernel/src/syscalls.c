// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <syscalls.h>
#include <keyboard.h>
#include <time.h>
#include <console.h>
#include <naiveConsole.h>
#include <sound.h>
#include <memory.h>
#include <process.h>

void read_handler(uint64_t fd, char * buff, uint64_t count) {
    // File descriptor doesn't matter
    for (int i = 0; i < count; i ++)
        buff[i] = read_character();
}

void write_handler(uint64_t fd, const char * buff, uint64_t count) {
    switch (fd) {
        case STDIN:
        case STDOUT:
            print_N(buff, count);
            //ncPrint(buff);
            break;
        case STDERR:
            printError_N(buff, count);
            //ncPrintError(buff);
            break;
        default:
            break;
    }
}

uint64_t time_handler() {
    return ticks_elapsed();
}

void clear_handler() {
    clear_console();
	// ncClear();
}

uint64_t rtc_handler(uint8_t reg){
    write_port(0x70, reg);
    uint64_t aux = read_port(0x71);
    return (aux >> 4) * 10 + (aux & 0x0F);
}

void sleep_handler(uint64_t millis){
    sleep(millis);
}

void beep_handler(uint16_t frequency, uint64_t millis) {
    play_sound(frequency);
    sleep_handler(millis);
    no_sound();
}

void pixel_handler(uint64_t x, uint64_t y, uint64_t rgb) {
    Vector2 auxPos = {x, y};
    Color auxColor = {(rgb & 0xFF0000) >> 16, (rgb & 0x00FF00) >> 8, rgb & 0x0000FF};
    draw_pixel(auxPos, auxColor);

    // ncPrintDec(rgb);
    // ncNewline();
    // ncPrintDec(auxColor.r);
    // ncNewline();
    // ncPrintDec(auxColor.g);
    // ncNewline();
    // ncPrintDec(auxColor.b);
    // ncNewline();
}

uint64_t memory_handler(uint8_t flag,uint64_t num){
    if(flag) {
        return free_block(num);
    }
    else {
        return allocate_blocks(num);
    }
}

uint64_t memory_state_handler(uint8_t free){
    if (free) {
        return cur_free_mem();
    }
    else {
        return total_mem();
    }
}

uint64_t create_process_handler(int priority) {
    return create_process(priority);
}