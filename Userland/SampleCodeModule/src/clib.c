// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <clib.h>
#include <utils.h>

#define READ_ID     0
#define WRITE_ID    1
#define TICKS_ID    2
#define CLEAR_ID    3
#define RTC_ID      4
#define SLEEP_ID    5
#define BEEP_ID     6
#define EXIT_ID     7
#define PIXEL_ID    8
//////////////////////////////////////////////
#define MEMORY_ALLOC_ID 9
#define MEMORY_STATE_ID 10
#define CREATE_PROCESS 11
#define UPDATE_PROCESS_PRIORITY 12
#define UPDATE_PROCESS_STATE 13
#define LIST_PROCESSES 14
#define LIST_SEM 15
#define LIST_PIPES 16
#define GET_PID 17
#define KILL_PROCESS 18
#define CREATE_PIPE 24
#define WRITE_PIPE 25
#define READ_PIPE 26
#define BLOCK_PROCESS 27
#define NICE 28
#define LOOP 29



#define STDIN       0
#define STDOUT      1
#define STDERR      2

#define MAX_BUFFER 100

static char * pipes_strings[] = {"no_pipe" , "pipe_1", "pipe_2", "pipe_3", "pipe_4", "pipe_5"};

static int outputs[MAX_PID];
static void *params[MAX_PID];
static int highest_pipe = 0;


void putchar(uint8_t character) {
    syscall(WRITE_ID, STDOUT, (uint64_t) &character, 1);
}

void puts(const char * string) {
    syscall(WRITE_ID, STDOUT, (uint64_t) string, strlen(string));
}

void perror(const char * string) {
    syscall(WRITE_ID, STDERR, (uint64_t) string, strlen(string));
}

void printf(char * str, ...){
    va_list list;
    va_start(list, str);
    int i = 0;

    char newStr[MAX_BUFFER] = {0};
    int len = 0;

    while(str[i] != 0){
    	if(str[i] == '%' && (i == 0 || str[i-1] != '\\')){
            char buffer[MAX_BUFFER] = {0};
            switch (str[i+1]) {
                case 'd':
                    itoa((int) va_arg(list,int), buffer, 10);
                    len += concat((newStr + len), buffer);
                    i += 2;
                    break;
                case 's':
                    len += concat((newStr + len), va_arg(list,char*));
                    i += 2;
                    break;
                default:
                    i += 2;
                    break;
            }
        }
        else {
            newStr[len] = str[i];
            len++;
            i++;
        }
    }
    va_end(list);
    newStr[len] = 0;
    len++;
    int pipe = outputs[get_pid()];
    if ( pipe ) {
        write_pipe(pipe, newStr );
    } else {
        syscall(WRITE_ID, STDOUT, (uint64_t) newStr, len);
    }
}

void printf_std(char * str, ...){
    // prints always on standard output.
    va_list list;
    va_start(list, str);
    int i = 0;

    char newStr[MAX_BUFFER] = {0};
    int len = 0;

    while(str[i] != 0){
    	if(str[i] == '%' && (i == 0 || str[i-1] != '\\')){
            char buffer[MAX_BUFFER] = {0};
            switch (str[i+1]) {
                case 'd':
                    itoa((int) va_arg(list,int), buffer, 10);
                    len += concat((newStr + len), buffer);
                    i += 2;
                    break;
                case 's':
                    len += concat((newStr + len), va_arg(list,char*));
                    i += 2;
                    break;
                default:
                    i += 2;
                    break;
            }
        }
        else {
            newStr[len] = str[i];
            len++;
            i++;
        }
    }
    va_end(list);
    newStr[len] = 0;
    len++;
    syscall(WRITE_ID, STDOUT, (uint64_t) newStr, len);
}

uint8_t getchar() {
    uint8_t character = 0;
    while (character == 0) syscall(READ_ID, STDIN, (uint64_t) &character, 1);
    return character;
}

// Obtiene un string de STDIN hasta ENTER o size
char * gets(char * string, uint64_t size) {
    uint64_t index = 0;
    uint8_t car;
    while (index < size - 1 && (car = getchar()) != '\n') {
        if (car == '\b') {
            if (index > 0) {
                index--;
                putchar(car);
            }
        } else {
            string[index++] = car;
            putchar(car);
        }
    }
    string[index] = 0;
    return string;
}

void clearScreen() {
    syscall(CLEAR_ID, 0, 0, 0);
}

// Formato de fecha: dd-mm-yyyy 
// date debe ser un vector de al menos 11 posiciones
char * getDate(char * date) {
    uint64_t day = syscall(RTC_ID, DAY_REG, 0, 0);
    uint64_t month = syscall(RTC_ID, MONTH_REG, 0, 0);
    uint64_t year = 2000 + syscall(RTC_ID, YEAR_REG, 0, 0); // Devuelve numero desde 2000

    saveTwoDigit(day, date);
    saveTwoDigit(month, date + 3);    
    itoa(year, date + 6, 10);

    date[2] = date[5] = DATE_SEP;
    date[10] = 0;

    return date;
}

// Formato de hora:  hh:mm:ss
// time debe ser un vector de al menos 9 posiciones
char * getTime(char * time) {
    uint64_t hour = syscall(RTC_ID, HOUR_REG, 0, 0);
    uint64_t min = syscall(RTC_ID, MIN_REG, 0, 0);
    uint64_t sec = syscall(RTC_ID, SEC_REG, 0, 0); // Devuelve numero desde 2000

    saveTwoDigit(hour, time);
    saveTwoDigit(min, time + 3);    
    saveTwoDigit(sec, time + 6);

    time[2] = time[5] = TIME_SEP;
    time[8] = 0;

    return time;
}

void sleep(uint64_t millis) {
    syscall(SLEEP_ID, millis, 0, 0);
}

void beep(uint32_t frequency, uint64_t millis) {
    syscall(BEEP_ID, frequency, millis, 0);
}

void exit() {
    syscall(EXIT_ID, 0, 0, 0);
}

uint64_t getTicks() {
    return syscall(TICKS_ID, 0, 0, 0);
}

void drawPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    uint64_t rgb = getRGB(r, g, b);
    syscall(PIXEL_ID, x, y, rgb);
}
////////////////////////////////////////////////////////////////////////////////////////// tp2_so
void *malloc(unsigned long bytes) {
    return (void *)syscall(MEMORY_ALLOC_ID, 0 ,bytes, 0);
}

int free(void *addr) {
    return syscall(MEMORY_ALLOC_ID, 1, (uint64_t) addr, 0);
}

void print_memstate() {
    uint64_t var = syscall(MEMORY_STATE_ID,0,0,0);
    uint64_t var2= syscall(MEMORY_STATE_ID,1,0,0);
    printf("Memoria total administrada: %d \nMemoria libre disponible: %d \n",var,var2);
}

int new_process(void *rip, const char *name, uint64_t param) {
    return syscall(CREATE_PROCESS, (uint64_t) rip , (uint64_t) name, (uint64_t) param);
}

uint64_t set_process_priority(int pid, int priority) {
    return syscall(UPDATE_PROCESS_PRIORITY, pid ,priority, 0);
}

uint64_t set_process_state(int pid, char state) {
    return syscall(UPDATE_PROCESS_STATE, pid ,state, 0);
}

uint64_t list_processes() {
    process_t **temp = (process_t **) syscall(LIST_PROCESSES, 0 ,0, 0);
    for(int i = 0 ; i < 50 ; i++ ) {
       if( temp[i] != NULL) {
           printf("Process called %s with pid %d is running and has priority %d \n",temp[i]->name, i, temp[i]->ppriority);
       }
    }
    output("End. \n");
    return 0;
}

uint64_t list_sem() {
    sem_t *temp = (sem_t *) syscall(LIST_SEM, 0 ,0, 0);
    if(temp == NULL){
        puts("No Sem Yet.");
        return 0;
    }
    p_blocked_t *blocked_p = temp->blocked_processes;
    int flag = 0;
    while(temp != NULL){
        flag = 0;
        puts("Sem ");
            printf("%d", temp->identifier);
            puts(" contains this blocked processes:");
            puts("/n");
        while(blocked_p != NULL){
            flag = 1;
            puts("Process ");
            printf("%d", blocked_p->pid);
            puts("/n");
        }
        if(!flag){
            puts("None");
        }
        temp=temp->next;
    }
    return 0;
}

uint64_t list_pipes() {
    pipe_t *aux = (pipe_t *) syscall(LIST_PIPES, 0 ,0, 0);
    if(aux == NULL){
        puts("No Pipes Yet.");
        return 0;
    }
    sem_t *temp = aux->global_sem;
    p_blocked_t *blocked_p = temp->blocked_processes;
    int flag = 0;
    while(aux != NULL){
        puts("Pipe ");
        printf("%d", aux->identifier);
        puts("contains this sem:");
        puts("/n");
        while(temp != NULL){
            flag = 0;
            puts("Sem ");
            printf("%d", temp->identifier);
            puts(" contains this blocked processes:");
            puts("/n");
            while(blocked_p != NULL){
                flag = 1;
                puts("Process ");
                printf("%d", blocked_p->pid);
                puts("/n");
            }
            if(!flag){
                puts("None");
            }
            temp=temp->next;
        }
        aux=aux->next;
    }
    return 0;

}

int create_pipe() {
    if (highest_pipe >= 4) {
        return -1;
    }
    int result = syscall(CREATE_PIPE, (uint64_t) pipes_strings[highest_pipe + 1], 0,0);
    if (result != 0) {
        return -1;
    }
    highest_pipe++;
    return highest_pipe;
}

int read_pipe(int pipe, char *str) {
    return syscall(READ_PIPE, (uint64_t) pipes_strings[pipe], (uint64_t) str,0);
    
}

int write_pipe(int pipe, char *str) {
    return syscall(WRITE_PIPE,(uint64_t) pipes_strings[pipe],(uint64_t) str, 0);
}

uint64_t kill_process(int pid) { 
    return syscall(KILL_PROCESS, pid,0, 0);
}

void block_process (int pid){ 
    int result =  syscall (BLOCK_PROCESS, pid, 0, 0);
    if(result == -1){
        output("No existe funcion con tal pid");
    } 
    else{
        output("Bloqueado con exito");
    }
}

void filter_input (char *input, char *dest) {
    int counter = 0,j=0;
    while ( *input != 0 && counter <= 2000) {
        if(*input != 'a' && *input != 'e' && *input != 'i' && *input != 'o' && *input != 'u') {
            *(dest+j) = *(input);
            j++;
        }
        input++;
        counter++;
    }
    *(dest+j) = 0;
    
}

int wc_input(char *input) {
    int counter = 0;
    int lines = 0;
    if (*input == 0 ) {
        return 0;
    }
    else {
        lines++;
    }
    while ( *input != 0 && counter <= 2000) {
        if(*input == '\n') {
           lines++;
        }
        
    }

    return lines;
}

void nice (int pid, int priority){
     syscall(NICE, pid, priority, 0);
}

void loop_function(){
     syscall(LOOP,0,0,0);
     puts("ANISMANLOMATARON");
}


int output(char *str) {
   int mypid = get_pid();
   if( outputs[mypid]) {
       write_pipe(outputs[mypid],str);
   }
   else {
      puts(str);
   }
   return 0;
}

void setOutput( int pid , int new_pipe ){
    outputs[pid] = new_pipe;
}

void setParam( int pid , uint64_t param ){
    params[pid] = (void *)param;
}

void *getParam( int pid ){
    return params[pid];
}

int get_pid() {
    return syscall(GET_PID,0,0,0);
}


//PHYLOS!!!

#define MODIFY_PHYLO_TABLE 30
#define GET_PHYLO_STATE 31
#define ADD_PHYLO 1
#define REMOVE_PHYLO 0

void add_philosopher(){
    return syscall(MODIFY_PHYLO_TABLE,ADD_PHYLO,0,0);
}

void remove_philosopher(){
    return syscall(MODIFY_PHYLO_TABLE,REMOVE_PHYLO,0,0);
}

int get_phylo_state(int i){
    return syscall(GET_PHYLO_STATE,i,0,0);
}
