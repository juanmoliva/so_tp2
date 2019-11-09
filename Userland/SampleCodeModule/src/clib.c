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
#define KILL_PROCESS 18
#define BLOCK_PROCESS 27
#define NICE 28
#define LOOP 29


#define STDIN       0
#define STDOUT      1
#define STDERR      2

#define MAX_BUFFER 100

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
    puts("Memoria total administrada: ");
    printf("%d", var);
    puts("\n");
    puts("Memoria libre disponible: ");
    printf("%d", syscall(MEMORY_STATE_ID,1,0,0));
    puts("\n");
}

void printSTDIN(){
    //IMPRIMIR EXACTAMENTE LO QUE RECIBE
}

int new_process(int priority,void *rip, const char *name) {
    return syscall(CREATE_PROCESS, (uint64_t) rip ,priority, (uint64_t) name);
}

uint64_t set_process_priority(int pid, int priority) {
    return syscall(UPDATE_PROCESS_PRIORITY, pid ,priority, 0);
}

uint64_t set_process_state(int pid, char state) {
    return syscall(UPDATE_PROCESS_STATE, pid ,state, 0);
}

uint64_t list_processes() {
    return syscall(LIST_PROCESSES, 0 ,0, 0);
}

uint64_t list_sem() {
    sem_t *temp = syscall(LIST_SEM, 0 ,0, 0);
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
        }
    }
    return 0;

}

uint64_t kill_process(int pid) { /////////////////////////////////////////PROBAR
    return syscall(KILL_PROCESS, pid,0, 0);
}

void block_process (int pid){ ////////////////////////////////////////////CAMBIAR PUTS POR OUTPUT MAYBE
    int result =  syscall (BLOCK_PROCESS, pid, 0, 0);
    if(result == -1){
        puts("No existe funcion con tal pid");
    } 
    else{
        puts("Bloqueado con exito");
    }
}

void filter_input (){

}

void nice (int pid, int priority){
     syscall(NICE, pid, priority, 0);
}

void loop_funtcion(){
     syscall(LOOP,0,0,0);
     puts("ANISMANLOMATARON");
}


output(char *str, char* pipe) {
   if(!strcmp(pipe,STD_OUT)) {
       // std_out
       puts(str);
   }
   else {
      write_pipe(pipe);
   }
}