// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <clib.h>
#include <shell.h>
#include <utils.h>

static char * command_strings[] = {"help", "date", "time", "sleep", "clear", "beep", "door", "div_zero", "inv_op", "exit", "ps",
                                   "sem","pipe","filter","wc","cat","block","kill","loop","nice","phylo","mem" };
static int command_count = 21;
void (* command_functions[]) (void) = {help_cmd, date_cmd, time_cmd, sleep_cmd, clear_cmd, beep_cmd, door_cmd, div_zero_cmd, inv_op_cmd, exit_cmd, ps_cmd
                                      ,sem_cmd, pipe_cmd, filter_cmd, wc_cmd, cat_cmd, block_cmd, kill_cmd, loop_cmd, nice_cmd, phylo_cmd, mem_cmd};

static int command_params_num[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0};

static int outputs[MAX_PID];


#define MAX_LENGTH  100
#define MAX_INPUT_LENGTH 2000
#define MEM_ADDRESS (void *)0x600000


static void newLine(){
    putchar('\n');
}
void initShell() {
    initScreen();
    int command = NO_CMD;
    char input[MAX_LENGTH];
    char shaved_input[MAX_LENGTH];
    int curr_in = 0; // pipes_strings[0] = no_pipe

    while (command != EXIT_CMD) {
        puts(PROMPT_STRING);
        gets(input, MAX_LENGTH);
        //FALTA SEPARAR EN ESPACIOS Y MANDARLE LA SALIDA DE UNO A LA ENTRADA DEL SIGUIENTE EN EL CASO DEL "cat | wc"
        int char_read = 0;
        int action;
        int end_of_input = 0;
        char command_input[MAX_INPUT_LENGTH];
        

        while( !end_of_input && command != EXIT_CMD ) {
            end_of_input = 1;
            action = UNDEFINED;
            command_input[0] = 0;
            int j = 0;
            for (; char_read < MAX_LENGTH-1; char_read++) {
                if ( input[char_read] != ' ' && input[char_read] != '\n' ) {
                    shaved_input[j++] = input[char_read];
                }
                else if (input[char_read] == '\n' ) {
                    //corre normal
                    action = END_OF_LINE;
                    break;
                } else {
                    break;
                }
            }
            shaved_input[j] = 0;
            
            // averiguamos el comando
            command = getCommand(shaved_input); 
            if (command == NO_CMD) {
                puts("error: lectura de comando erroneo. ");
                break;
            }

            // definimos la accion.
            if( action != END_OF_LINE ) {
                char_read++;
                if ( input[char_read] == '#' ) { action = PIPE; char_read; end_of_input=0;}
                else if ( input[char_read] == '&') {action = BACKGROUND; char_read++;}
                else { action = PARAMS; }
            }

            // de donde debo leer el input?
            if ( command_params_num[command] != 0 ) {
                if( action == PARAMS ) {
                        // leo parametro.
                        int h=0;
                        while(input[char_read] != '\n' && input[char_read] != ' ') {
                            command_input[h++] = input[char_read];
                            char_read++;
                        }
                        command_input[h] = 0;
                        
                } else if ( curr_in ) {
                    // el input se recibirá de un pipe.
                    read_pipe(curr_in, command_input);
                }
                else {
                    puts("A command did not receive required input.");
                    command = NO_CMD;
                    break;
                }
            } else {
                curr_in = 0;
            }
            
            // llegados a este punto, si el comando requiere parametros/input los tengo en command_input
            // ahora solo queda ejecutar el comando, e indicarle el pipe/output donde tiene que escribir.

            if ( action == BACKGROUND ) {
                // el comando lo ejecutamos en un nuevo proceso, el output lo escribimos en la consola.
                new_process(&command_functions[command], command_strings[command], command_input);
                end_of_input = 1;
            } else if (action == PIPE ) {
                // creamos un nuevo pipe, un proceso nuevo que escriba en ese pipe.
                int new_pipe = create_pipe();
                if (new_pipe == -1)  {
                    puts("Error. El programa llego al numero máximo de pipes en uso.");
                    command = NO_CMD;
                    break;
                }                
                int pid = new_process(&command_functions[command], command_strings[command], command_input);
                outputs[pid] = new_pipe;
            } else {
                end_of_input = 1;
                executeCommand(command);
            }
        }

        /* // user escribe " ps | cat "
        for (int i = 0; i < MAX_LENGTH-1; i++)
        {
            if ( input[i] != ' ' && input[i] != '\n' ) {
                char_read++;
                shaved_input[j++] = input[i];
            }
            else if (input[i] == '\n' ) {
                //corre normal
                command_only = 1;
                break;
            } else {
                break;
            }
        }

        // char_read = 2 porque ps tiene longitud dos
        // ahora nos fijamos que hay en el indice char_read (que hay despues del espacio)

        // qué parametro se leyó?
        if(command_only){
            command = getCommand(input); 
        }
        else{
            command = getCommand(shaved_input); 
        }

        if ( command_params_num == 1 ) {

        } else if( command_params_num == 2 ){

        } else {

        }

        if ( curr_in ) {
            // el input debe leerse de un pipe.
        }
        
        //Aca cae solo si NO es un comando solo, es decir, tiene otro cosa dps de un PIPE
        if ( !command_only ) { 
            if ( input[char_read] == '#') {
                // pipe
                // crear pipe  --> 
                // int pipe_result = create_pipe()

                
            } else if ( input[char_read] == '&' ){
                // background
                // si piden background creamos un nuevo proceso con la funcion que piden, si no ejecutamos la funcion.
                new_process = 1;
            } else {
                //parametro
            }
        } else if ( command_params_num[command] > 0 ) {
            /
        }
        
        if ( new_process ){ 
           //
           // new_process(3, &command, command_strings[command], param);
        }
        else {
            // 
            // executeCommand(command, params,in, out);
        }*/
    }
	if (command != CLEAR_CMD) newLine();
	exit();
}

readParams() {

}

void initScreen() {
    clearScreen();   
}
int getCommand(char * input){
    for (int i = 0; i < command_count; i++) {
        if (strcmp(input, command_strings[i]) == 0)
            return i;
    }
    return NO_CMD;
}
void executeCommand(int command) {
    if (command != NO_CMD)
        command_functions[command]();
    else
        puts("\nInvalid command");
}
void help_cmd() {
    puts("\nLos comandos validos son los siguientes: ");
    puts("\nhelp ~ Muestra los comandos validos");
    puts("\ndate ~ Muestra la fecha actual");
    puts("\ntime ~ Muestra la hora actual");
    puts("\nsleep ~ Frena el funcionamiento un numero de segundos a ingresar");
    puts("\nclear ~ Limpia la consola");
    puts("\nbeep ~ Emite un sonido");
    puts("\ndoor ~ Hay alguien en la puerta");
    puts("\ndiv_zero ~ Ejecuta una division por cero");
    puts("\ninv_op ~ Ejecuta una operacion de codigo invalido ");
    puts("\nexit ~ Termina la ejecucion");
    puts("\nps ~ lista todos los procesos");
    puts("\nsem ~ lista todos los semaforos ");
    puts("\npipe ~ lista todos los pipes");


}
void date_cmd() {
    char date[11];
    printf("\nHoy es %s", getDate(date));
    // puts("\nHoy es  ");
	// puts(getDate(date));
}
void time_cmd() {
    char time[9];
    puts("\nSon las  ");
	puts(getTime(time));
}
void sleep_cmd() {
    char car;
    do{
        puts("\nIngrese el numero de segundos que desea esperar [0-9]: ");
        car = getchar();
        putchar(car);
    } while (!isNumber(car));
    int millis = (car - '0') * 1000;
    sleep(millis);
}
void clear_cmd() {
    clearScreen();
}
void beep_cmd() {
    beep(BEEP_FREQ, 300);
}
void door_cmd() {
    beep(DOOR_FREQ, 300);
    sleep(300);
    beep(DOOR_FREQ, 150);
    sleep(150);
    beep(DOOR_FREQ, 150);
    sleep(100);
    beep(DOOR_FREQ, 150);
    sleep(250);
    beep(DOOR_FREQ, 150);

    sleep(800);
    beep(DOOR_FREQ, 150);
    sleep(150);
    beep(DOOR_FREQ, 150);
}
void div_zero_cmd() {
    int a = 10, b = 0;
    a = a / b;
    printf("%d", a);
}
void inv_op_cmd() {
    uint64_t invalid = 0xFFFFFFFFFFFF;
	uint64_t * ptr = &invalid;
	((void(*)())ptr)();
}
void exit_cmd() {
    puts("\nHasta Luego");
}

////////// tp2_so

/* void ps_cmd(){  //// FALOPA , FALTA RETOCAR PERO EN PRINCIPIO MASOMENOS VA 
   char** to_print = list_processes();
   while (to_print** != "enofstring"){
       puts(to_print**);
       char to_print++;
   }
} */
void sem_cmd(){ 
    list_sem();
}
void pipe_cmd(){
    list_pipes();
}
void filter_cmd(){
    filter_input();
}
void wc_cmd(){
}

void ps_cmd(){
}
void cat_cmd(){
    printSTDIN();
}
void block_cmd(int pid){
    // USE CASE: block 34 ---> bloquea el proceso de pid 34
    // Cambia el estado de un proceso entre ​bloqueado​ y ​listo​ dado su ID.
    block_process(pid);
}
void kill_cmd(int id){
    kill_process(id);
}
void loop_cmd(){
}
void nice_cmd(int pid, int priority){
    nice(pid, priority);
}
void phylo_cmd(){
}
void mem_cmd(){
  print_memstate();
}

