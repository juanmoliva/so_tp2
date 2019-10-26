#ifndef SHELL_H_
#define SHELL_H_

#define NO_CMD          -1
#define HELP_CMD        0
#define DATE_CMD        1
#define TIME_CMD        2
#define SLEEP_CMD       3
#define CLEAR_CMD       4
#define BEEP_CMD        5
#define DOOR_CMD        6
#define DIV_ZERO_CMD    7
#define INV_OP_CMD      8
#define EXIT_CMD        9

#define PROMPT_STRING   "mjj@itba.edu.ar $ "
#define BEEP_FREQ       400
#define DOOR_FREQ       600

void initShell();
void initScreen();
int getCommand(char * input);
void executeCommand(int command);

void help_cmd();
void date_cmd();
void time_cmd();
void sleep_cmd();
void clear_cmd();
void beep_cmd();
void door_cmd();
void div_zero_cmd();
void inv_op_cmd();
void exit_cmd();

#endif /* SHELL_H_ */