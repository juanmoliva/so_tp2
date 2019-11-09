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

static char * pipes_strings[] = {"pipe_1", "pipe_2", "pipe_3", "pipe_4", "pipe_5"}
#define MAX_LENGTH  50
#define MEM_ADDRESS (void *)0x600000
#define STD_IN "std_in"
#define STD_OUT "std_out"

static void newLine(){
    putchar('\n');
}

void initShell() {
    initScreen();
    int command = NO_CMD;
    char input[MAX_LENGTH];
    char shaved_input[MAX_LENGTH];

    while (command != EXIT_CMD) {
        puts(PROMPT_STRING);
        gets(input, MAX_LENGTH);
        //FALTA SEPARAR EN ESPACIOS Y MANDARLE LA SALIDA DE UNO A LA ENTRADA DEL SIGUIENTE EN EL CASO DEL "cat | wc"
        int char_read = 0;
        int command_only = 0;
        int new_process = 1;
        int j = 0;
        // user escribe " ps | cat "
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
            }
            else {
                break;
            }
        }

        // char_read = 2 porque ps tiene longitud dos
        // ahora nos fijamos que hay en el indice char_read+1 (que hay despues del espacio)

        //Aca cae solo si NO es un comando solo, es decir, tiene otro cosa dps de un PIPE
        if ( !command_only ) { 
            if ( input[char_read] == '|') {
                // pipe
                // crear pipe  --> 
                int pipe_result = create_pipe()

                
            } else if ( input[char_read] == '&' ){
                // background
                // si piden background creamos un nuevo proceso con la funcion que piden, si no ejecutamos la funcion.
                new_process = 1;
            } else {
                //parametro
            }
        }


        
        if(command_only){
            command = getCommand(input); 
        }
        else{
            command = getCommand(shaved_input); 
        }
        
        if ( new_process ){ 
           //
           // new_process(3, &command, command_strings[command], param);
        }
        else {
            // 
            // executeCommand(command, params,in, out);
        }
       
        
        if (command != CLEAR_CMD) newLine();
    }
	
	exit();
}




void initScreen() {
    clearScreen();   
}

int readUntilSpace(char *input) {
    // lee input hasta el espacio y devuelve la cantidad de chars que se leyeron
}

int readAfterCommand(char *input, int index) {
    // lee input + index y se fija lo que hay despues (del comando)
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

void ps_cmd(){  //// FALOPA , FALTA RETOCAR PERO EN PRINCIPIO MASOMENOS VA 
   char** to_print = list_processes();
   while (to_print** != "enofstring"){
       puts(to_print**);
       char to_print++;
   }
}

void sem_cmd(){ 
    list_sem();
}

void pipe_cmd(){list_pipes();
}

void filter_cmd(){
    filter_input();
}

void wc_cmd(){
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
    loop_function();
}

void nice_cmd(int pid, int priority){
    nice(pid, priority);
}

void phylo_cmd(){
}

void mem_cmd(){
  print_memstate();
}


// output( "hola", STD_OUT ){ 

// }
// output(char *str, char* pipe) {
//    if(!strcmp(pipe,STD_OUT)) {
//        // std_out
//        puts(str);
//    }
//    else {
//        write_pipe(pipe);
//    }
// }