// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <sound.h>

void play_sound(uint32_t frequency) {
 	
     //Set the PIT to the desired frequency
    uint32_t div = 1193180 / frequency;
 	uint8_t aux;

 	write_port(0x43, 0xB6);
 	write_port(0x42, (uint8_t) (div) );
 	write_port(0x42, (uint8_t) (div >> 8));
 
    //Playing the sound using the PC speaker
 	aux = read_port(0x61);
  	if (aux != (aux | 3))
 		write_port(0x61, aux | 3);
 }
 
 //Stop sound
 void no_sound() {
 	uint8_t aux = read_port(0x61) & 0xFC;
 	write_port(0x61, aux);
 }
