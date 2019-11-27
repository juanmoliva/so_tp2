// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <clib.h>
#include <shell.h>
#include <utils.h>

static char * command_strings[] = {"help", "date", "time", "sleep", "clear", "beep", "door", "div_zero", "inv_op", "exit", "ps",
                                   "sem","pipe","filter","wc","cat","block","kill","loop","nice","phylo","mem" };
static int command_count = 22;
void (* command_functions[]) (void *param) = {help_cmd, date_cmd, time_cmd, sleep_cmd, clear_cmd, beep_cmd, door_cmd, div_zero_cmd, inv_op_cmd, exit_cmd, ps_cmd
                                      ,sem_cmd, pipe_cmd, filter_cmd, wc_cmd, cat_cmd, block_cmd, kill_cmd, loop_cmd, nice_cmd, phylo_cmd, mem_cmd};

static int command_params_num[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0};

void new_process_wrapper( int command );
int shell_reader( char *input, char *dest, int *offset );
void shell_loop();


#define MAX_LENGTH  100
#define MAX_INPUT_LENGTH 2000
#define MEM_ADDRESS (void *)0x600000


static void newLine(){
    putchar('\n');
}
void initShell() {
    initScreen();
    shell_loop();
}

void shell_loop() {
    int command = NO_CMD;
    char input[MAX_LENGTH];
    char shaved_input[MAX_LENGTH];
    int curr_in = 0; // pipes_strings[0] = no_pipe

    while (command != EXIT_CMD) {
        puts(PROMPT_STRING);
        gets(input, MAX_LENGTH);
        int char_read = 0;
        int action;
        int end_of_input = 0;
        void *param;
        char command_input[MAX_INPUT_LENGTH];
        

        while( !end_of_input && command != EXIT_CMD ) {
            newLine();
            end_of_input = 1;
            action = UNDEFINED;
            command_input[0] = 0;
            param = (void *)0x0;

            int res = shell_reader(input, shaved_input, &char_read);
            /* int j = 0;
            for (; char_read < MAX_LENGTH-1; char_read++) {
                if ( input[char_read] != ' ' && input[char_read] != '\n' && input[char_read] != '\0' ) {
                    shaved_input[j++] = input[char_read];
                }
                else if (input[char_read] == '\n' || input[char_read] == '\0' ) {
                    //corre normal
                    action = END_OF_LINE;
                    break;
                } else {
                    break;
                }
            }
            shaved_input[j] = 0; */
            if (res < 0) {
                printf_std("error: error inesperado. \n");
                break;
            }
            if ( !res ) {
                //corre normal
                action = END_OF_LINE;
            }
            
            // averiguamos el comando
            command = getCommand(shaved_input); 
            if (command == NO_CMD) {
                puts("error: lectura de comando erroneo. \n");
                break;
            }
            
            // definimos la accion.
            if( action != END_OF_LINE ) {
                char_read++;
                int res_2 = shell_reader(input, command_input, &char_read);
                if( res_2 == -1 ) {
                    printf_std("error: error inesperado. \n");
                    break;
                }
                else if (!res_2) {
                    if ( command_input[0] == '#' ) { action = PIPE; char_read++; end_of_input=0;}
                    else if ( command_input[0] == '&') {action = BACKGROUND; char_read++;}
                    else { 
                        action = PARAMS;
                        param = malloc(50);
                        memcpy(param, command_input, 50);
                    }
                }
                else {
                    // space after input.
                    if(input[char_read + 1] == '#') {
                        param = malloc(50);
                        memcpy(param, command_input, 50);
                        action = PIPE; char_read = char_read +2; end_of_input=0;
                    }
                    else if(input[char_read + 1] == '&') {
                        param = malloc(50);
                        memcpy(param, command_input, 50);
                        action = BACKGROUND; char_read = char_read +2;
                    }
                    else {
                        printf_std("error: lectura/cantidad de parametros erronea. \n");
                        break;
                    }
                }
                
            }

            /* // definimos la accion.
            if( action != END_OF_LINE ) {
                char_read++;
                if ( input[char_read] == '#' ) { action = PIPE; char_read++; end_of_input=0;}
                else if ( input[char_read] == '&') {action = BACKGROUND; char_read++;}
                else { action = PARAMS; }
            } */

            // de donde debo leer el input?
            int h=0;
            if ( command_params_num[command] != 0 ) {
                if ( curr_in ) {
                    // el input se recibirá de un pipe.
                    sleep(3000);
                    puts("reading from pipe \n");
                    read_pipe(curr_in, command_input);
                    puts("read from pipe: ");
                    puts(command_input);
                    puts("\n");
                    for(;h<MAX_INPUT_LENGTH && command_input[h] != 0; h++){}
                } else if( !param ) {
                        puts("A command did not receive required input. \n");
                        command = NO_CMD;
                        break;
                }        // leo parametro.
                        /* 
                        while(input[char_read] != '\n' && input[char_read] != ' ' && input[char_read] != '\0') {
                            command_input[h++] = input[char_read];
                            char_read++;
                        }
                        command_input[h] = 0;
                        
                } 
                else {
                    puts("A command did not receive required input. \n");
                    command = NO_CMD;
                    break;
                } */
            } else {
                curr_in = 0;
            }
            
            // ahora solo queda ejecutar el comando, e indicarle el pipe/output donde tiene que escribir.
            
            /* if (command_input[0] != 0) {
                param = malloc(h);
                memcpy(param, command_input, h);
            } */
            if ( action == BACKGROUND ) {
                // el comando lo ejecutamos en un nuevo proceso, el output lo escribimos en la consola.
                puts("proceso ejecutandose en background! \n");
                int new_pid = new_process(&new_process_wrapper, command_strings[command],(uint64_t) command);
                printf("nuevo pid: %d \n", new_pid);
                setParam(new_pid,(uint64_t) param);
                end_of_input = 1;
            } else if (action == PIPE ) {
                // creamos un nuevo pipe, un proceso nuevo que escriba en ese pipe.
                int new_pipe = create_pipe();
                puts("a pipe was created: ");
                printf("%d", new_pipe);
                if (new_pipe == -1)  {
                    puts("Error. El programa llego al numero máximo de pipes en uso. \n");
                    command = NO_CMD;
                    break;
                }            
                curr_in = new_pipe;    
                int new_pid = new_process(&new_process_wrapper, command_strings[command], (uint64_t)command);
                setOutput(new_pid,new_pipe);
                setParam(new_pid,(uint64_t) param);
                char_read++; // espacio
            } else {
                end_of_input = 1;
                executeCommand(command, param);
            }

            if (command != CLEAR_CMD) newLine();
        }
    }
	exit();
}

int shell_reader( char *input, char *dest, int *offset ) {
    // reads input, returns 1 if there is a space after text read.
    // returns 0 if after text there is a \n or  \0.

    int j = 0;
    for (; (*offset) < MAX_LENGTH-1; (*offset)++) {
        if ( input[*offset] != ' ' && input[*offset] != '\n' && input[*offset] != '\0' ) {
            dest[j++] = input[*offset];
        }
        else if (input[*offset] == '\n' || input[*offset] == '\0' ) {
            // not space
            dest[j] = 0;
            return 0;
        } else {
            // space
            dest[j] = 0;
            return 1;
        }
    }
    // error
    return -1;
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
void executeCommand(int command, void *param) {
    if (command != NO_CMD) {
        command_functions[command](param);
        free(param); 
    }
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
    puts("\ncat ~ redirige output");
    puts("\nwc ~ cuenta lineas del input");
    puts("\nfilter ~ filtra vocales del input");
    puts("\nblock <pid> ~ bloquea un proceso");
    puts("\nkill <pid> ~ mata un proceso");
    puts("\nnice <pid> ~ cambia la prioridad de un proceso");
    puts("\nloop <X> ~ Crea un proceso que imprime su pid cada X segundos");
    puts("\npara utilizar un pipe utilice el simbolo '#' ");


}
void date_cmd() {
    char date[11];
    printf("\nHoy es %s", getDate(date));
    // puts("\nHoy es  ");
	// puts(getDate(date));
}
void time_cmd() {
    char time[9];
    output("\nSon las  ");
	output(getTime(time));
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

void ps_cmd(){ 
    list_processes();
}
void sem_cmd(){ 
    list_sem();
}

/* void pipe_cmd(){
    list_pipes();
} */

void pipe_cmd() {
    // creamos un nuevo pipe, un proceso nuevo que escriba en ese pipe.
    int new_pipe = create_pipe();
    puts(" will execute ps # filter");
    puts("a pipe was created: ");
    printf("%d\n", new_pipe);
    int new_pid = new_process(&new_process_wrapper, command_strings[10], (uint64_t)10);
    setOutput(new_pid,new_pipe);
    setParam(new_pid,(uint64_t) 0x0);


    sleep(3000);

    char input[500];
    puts("reading from pipe \n");
    read_pipe(new_pipe, input);
    puts("read from pipe: ");
    puts(input);
    puts("\n");

    filter_cmd((void *)input);
}


void filter_cmd(void *input){
    if (input == NULL) {
        output(" \ninvalid input for filter. \n");
        return;
    }
    char dest[500];
    puts("input received: \n");
    puts((char *)input);
    puts("\n");
    filter_input( (char *)input, dest );
    puts("output from filter: \n");
    output(dest);
}

void wc_cmd(void *input){
    if (input == NULL) {
        output(" \ninvalid input for wc. \n");
        return;
    }
    printf("%d lines. \n", wc_input((char *)input));
}

void cat_cmd(void *param){
    output((char *)param);
}
void block_cmd(void *pid){
    // USE CASE: block 34 ---> bloquea el proceso de pid 34
    // Cambia el estado de un proceso entre ​bloqueado​ y ​listo​ dado su ID.
    int mypid = atoi(pid,1);
    if( mypid > 50 || mypid< 0) { puts("Invalid pid for block. \n"); return; }
    block_process(mypid);
}
void kill_cmd(void *pid){
    int mypid = atoi(pid,1);
    if( mypid > 50 || mypid< 0) { puts("Invalid pid for pid. \n"); return; }
    printf("About to kill process %d \n", mypid);
    kill_process(mypid);
}

void nice_cmd(void *pid){
    int mypid = atoi(pid,1);
    printf("%d \n", mypid);
    if( mypid > 50 || mypid< 0) { puts("Invalid pid for nice. \n"); return; }
    printf_std("Select new priority for process with pid: %d \n", mypid);
    char priority[MAX_LENGTH];
    gets(priority, MAX_LENGTH);
    priority[5] = 0;
    int prior = atoi(priority, 1);
    if( prior < 0 || prior > 10) {
        printf_std("invalid priority: must be between 0 and 10 \n");
        return;
    }
    nice(mypid, prior );
    return;
}

void loop_cmd(void *secs){
    int loop_secs = atoi(secs,1);
    int my_pid = get_pid();
    
    while(1) {
        printf("Im process with pid %d \n", my_pid);
        sleep(loop_secs*1000);
    }
    

}

void phylo_cmd(){
    char *states[] = {"thinking", "hungry", "eating"};
    int curr_phylos = 1;
    add_philosopher();
    printf_std("\nel problema de los filosofos comensales. \n");
    printf_std("\ncurrent philosophers: %d \n", curr_phylos);

   
    printf_std("Select:\n\t 's' to see table's state.\n");
    printf_std("\t'a' to add a philosopher.\n");
    printf_std("\t'r' to remove a philosopher.\n");
    printf_std("\t'p' to run ps command (see currently running processes).\n");
    printf_std("\t'q' to quit program.\n");
    char input[50];
    while (1) {
            printf_std("\n---- ");
            gets(input, 50);
            printf_std("\n\n");
        if ( input[0] == 'a' ) {
            add_philosopher();
            curr_phylos++;
            printf_std("current philosophers: %d \n", curr_phylos);
        }else if (input[0] == 's')
        {
            int state;
            for (int i = 0; i < curr_phylos; i++)
            {
                //get state
                state = get_phylo_state(i);
                //print
                printf_std("\nPhylo - %d =",i);
                printf_std(" %s",states[state]);
            }
            printf_std("\n");
            //Phylo-1 = Thinking
            //Phylo-2 = Eating
            //Phylo-3 = Eating
            //Phylo-4 = Thinking
            //Phylo-5 = Hungry

        }else if (input[0] == 'r')
        {
            remove_philosopher();
            curr_phylos--;
            printf_std("current philosophers: %d \n", curr_phylos);
        }else if (input[0] == 'p')
        {
            ps_cmd();
        }else if (input[0]=='q')
        {
            for (int i = 0; i<curr_phylos; i++) {
                remove_philosopher();
            }
            return;
        }
    }
}
void mem_cmd(){
  print_memstate();
}

void new_process_wrapper( int command ) {
    int pid = get_pid();
    void *param = getParam( pid );
    command_functions[command](param);

    kill_process(pid);
    shell_loop();
}