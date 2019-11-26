#ifndef PHYLO_H
#define PHYLO_H

#define N 5 /* number of philosophers */
#define LEFT (i+N-1)%N /* number of i’s left neighbor */
#define RIGHT (i+1)%N /* number of i’s right neighbor */
#define THINKING 0 /* philosopher is thinking */
#define HUNGRY 1 /* philosopher is trying to get forks */
#define EATING 2 /* philosopher is eating */

void philosopher(int i);
void take_forks(int i);
void put_forks(int i);
void test(int i);

#endif