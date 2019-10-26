// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <keyboard.h>
#include <lib.h>
#include <naiveConsole.h>

#define BUFFER_SIZE 50

char buffer[BUFFER_SIZE];
unsigned int save_index = 0;
unsigned int read_index = 0;

static unsigned char map[90]={'`','\e','1','2','3','4','5','6','7','8','9','0','-','=','\b','\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
        0/*CTRL*/,'a','s','d','f','g','h','j','k','l',';','\'', 0, 'S','\\','z','x','c','v','b','n','m',',','.','/','S',0,0/*ALT*/,' ',
        'M',0,0,0,0,0,0,0,0,0,0,0,0,0,UP_KEY,0,0,LEFT_KEY,0,RIGHT_KEY,0,0,DOWN_KEY };

static unsigned char shift_map[90]={'~','\e','!','@','#','$','%','^','&','*','(',')','_','+','\b','\t','q','w','e','r','t','y','u','i','o','p','{','}','\n',
        0/*CTRL*/,'a','s','d','f','g','h','j','k','l',':','\"', 0, 'S','|','z','x','c','v','b','n','m','<','>','?','S',0,0/*ALT*/,' ',
        'M',0,0,0,0,0,0,0,0,0,0,0,0,0,UP_KEY,0,0,LEFT_KEY,0,RIGHT_KEY,0,0,DOWN_KEY };

// ESC = \e
// BACK = \b
// SHIFT = S
// CAPS = M
// CTRL = C
// ALT = A
// UP = U
// LEFT = L
// RIGHT = R
// DOWN = D

// Solo chequeo si presione SHIFT o MAY, los demas pasan como vienen
void keyboard_handler() {
    static int caps = 1, shift = 1, release_caps = 1;
    unsigned char key = read_port(0x60);

    if ((key >= 90 && key < 0x80) || key >= 0x80 + 90)
        return;

    unsigned char car;
    if (key < 0x80){ // Aprieto tecla
        if (shift == 1)
            car = map[key];
        else
            car = shift_map[key];
        switch (car){
            case 'S':   
                shift = -1;
                break;
            case 'M':   
                if (release_caps){
                    caps *= -1;
                    release_caps = 0;
                }
                break;
            default:
                if (shift * caps == -1){ // Uno de los dos activos, paso a mayus
                    buffer[save_index % BUFFER_SIZE] = toUpper(car);
                    //printChar(toUpper(car), 0x0F);
                } else{
                    buffer[save_index % BUFFER_SIZE] = car;
                    //printChar(car, 0x0F);
                }
                save_index++;
                break;
        }
    } else { // Suelto tecla
        car = map[key - 0x80];
        switch (car){
            case 'S':
                shift = 1;
                break;
            case 'M':
                release_caps = 1;
                break;
        }
    }

    // char key = map2[read_port(0x60)];
    // if (key > 0) {
    //     buffer[index++] = key;  
    // }
    // ncPrintHex(read_port(0x60));
    // ncPrint(" - ");
}

char toUpper(char car){
    if (car >= 'a' && car <= 'z')
        car -= ('a' - 'A');
    return car;
}

char read_character() {
    if (read_index == save_index) return 0;
    unsigned char character = buffer[read_index % BUFFER_SIZE];
    read_index++;    
    return character;
}