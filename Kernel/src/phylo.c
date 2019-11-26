#include <phylo.h>
#include <synchro.h>
#include <process.h>

#define ID_PHYLO 300 

int state[N]; /* array to keep track of everyone’s state */
int pids[N];
sem_t *mutex;  /* mutual exclusion for critical regions */
sem_t *s[N]; /* one semaphore per philosopher */
int curr_phylos = 0;

int init_phylo() {
    int res = sem_init(ID_PHYLO + 1, 1);
    if (res != 0) {
        return 1; //error
    }
    mutex = sem_open(ID_PHYLO + 1);

}

void philosopher(int i) {   /* i: philosopher number, from 0 to N−1 */
    
    int res = sem_init(ID_PHYLO + i*ID_PHYLO, 0);
    if (res != 0) {
        return 1; //error
    }
    s[i] = sem_open(ID_PHYLO + i*ID_PHYLO);

    while (1) {             /* repeat forever */
        think(i);            /* philosopher is thinking */
        take_forks(i);      /* acquire two forks or block */ 
        eat(i);              /* yum-yum, spaghetti */
        put_forks(i);       /* put both forks back on table */
    } 
}

void think(i){
    
    state[i] = THINKING;
    sleep_handler(2000);
}

void eat(i){
    state[i] = EATING;
    sleep_handler(2000);
}

void take_forks(int i)      /* i: philosopher number, from 0 to N−1 */
{    
    //down(&mutex);           /* enter critical region */
    sem_wait(mutex);

    state[i] = HUNGRY;      /* record fact that philosopher i is hungry */
    test(i);                /* try to acquire 2 forks */

    //up(&mutex);             /* exit critical region */
    sem_post(mutex);

   // down(&s[i]);            /* block if forks were not acquired */
    sem_wait(s[i]);
}

void put_forks(int i) {     /* i: philosopher number, from 0 to N−1 */
    //down(&mutex);           /* enter critical region */
    sem_wait(mutex);

    state[i] = THINKING;    /* philosopher has finished eating */
    test(LEFT);             /* see if left neighbor can now eat */
    test(RIGHT);            /* see if right neighbor can now eat */
    //up(&mutex);             /* exit critical region */
    sem_post(mutex);
 }


void test(int i) /* i: philosopher number, from 0 to N−1 */ 
{
    if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) { 
        state[i] = EATING;
        sem_post(s[i]);
    }
}

int get_state_phylosopher(int i) {
    return state[i];
}

int add_phylo(){
    if ( curr_phylos == 0 ){
        init_phylo();
    }
    if (curr_phylos >= N ) {
        return 1;
    } else {
        pids[curr_phylos] = create_process(5, &philosopher, "philosopher", curr_phylos);
        curr_phylos++;
        return 0;
    }
}

int remove_phylo(){
    if ( curr_phylos == 0 ) {
        return 1;
    }
    kill_process(pids[curr_phylos]);
    sem_close(s[curr_phylos]);
    curr_phylos--;
    return 0;
}