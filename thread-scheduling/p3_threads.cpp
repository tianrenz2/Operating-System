#include "p3_threads.h"
#include "utils.h"
 
extern pthread_mutex_t mutex;
extern pthread_mutex_t taskDoneMutex;

extern pthread_cond_t  cond[4];
extern pthread_cond_t a_task_is_done;
extern std::vector<int> readyQue;
extern int occupied;
extern int num_of_alive_tasks;

extern int global_work; // DO NOT INITIALIZE!!

void *threadfunc(void *param)
{

	long stamp;
	ThreadCtrlBlk* tcb = (ThreadCtrlBlk*)param;	
	int iter = 1;
	int id = tcb->id;
	int sleep = 0;
	int fail = 0;

	printf(" - [Thread %d] Start\n", id);

	while (true) {
		pthread_mutex_lock(&mutex);
		if (global_work==0) {
			pthread_mutex_unlock(&mutex);
			break;
		}
		// push id
		readyQue.push_back(id);
		stamp = get_time_stamp();
		printf("[%lu ms][Thread %d] Goto sleep\n", stamp, id);
		
		
		// goes into sleep and waits for a signal from scheduler
		while (true){
			pthread_cond_wait(&(cond[id]), &mutex);
			if (occupied==0) break;
		}
		
		if (global_work==0) {
			pthread_mutex_unlock(&mutex);
			break;
		}
		
		occupied = 1;
		pthread_mutex_unlock(&mutex);
		
		
		// Starts Executing
		if (global_work==0) { 
			pthread_mutex_lock(&mutex);
			occupied = 0;
			pthread_mutex_unlock(&mutex);
			pthread_mutex_lock(&taskDoneMutex);
			pthread_cond_signal(&a_task_is_done);
			pthread_mutex_unlock(&taskDoneMutex);
		
		break;}

		// unlock & block
		stamp = get_time_stamp();
		printf("[%lu ms][Thread %d] Start Task (%d, %d) (%lu)ms\n", stamp, id, id,iter, tcb->task_time);
		// perform task
		usleep(MSEC(tcb->task_time));
		stamp = get_time_stamp();
		printf("[%lu ms][Thread %d] Complete Task (%d, %d) (%lu)ms\n", stamp, id, id,iter, tcb->task_time);

		if (stamp > (tcb->deadline + 50)) {
			printf("[%lu ms][Thread %d] Task (%d, %d) MISS the DEADLINE!!!! (Deadline: %lu)\n", stamp, id, id, iter, tcb->deadline);
			fail = 1;
			pthread_mutex_lock(&mutex);
			occupied = 0;
			pthread_mutex_unlock(&mutex);
			pthread_mutex_lock(&taskDoneMutex);
			pthread_cond_signal(&a_task_is_done);
			pthread_mutex_unlock(&taskDoneMutex);
			break;
		}
		

		
		
		// estimate waiting time
		stamp = get_time_stamp();
		sleep = iter * tcb->period - stamp;
		iter++;

		// set deadline for next task and realease change occupied value to availble
		pthread_mutex_lock(&mutex);
		tcb->deadline = iter * tcb->period;
		occupied = 0;
		pthread_mutex_unlock(&mutex);
		
		
		pthread_mutex_lock(&taskDoneMutex);
		pthread_cond_signal(&a_task_is_done);
		pthread_mutex_unlock(&taskDoneMutex);

		if(sleep > 0) 
			usleep(MSEC(sleep));

	}


	printf(" - [Thread %d] Complete\n", id);
	if (fail) {
		num_of_alive_tasks--;
		printf(" - [Thread %d] MISS the DEADLINE\n", id);
	}
}
