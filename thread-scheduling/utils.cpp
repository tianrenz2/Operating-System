#include "utils.h"

extern struct timeval t_global_start;
extern pthread_mutex_t taskDoneMutex;
extern pthread_cond_t a_task_is_done;

long get_elasped_time(struct timeval& start, struct timeval& end)
{

	long mtime = 0;
    long seconds  = end.tv_sec  - start.tv_sec;
    long useconds = end.tv_usec - start.tv_usec;

    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;

	return mtime;
}
 

long get_time_stamp(void)
{
	struct timeval t_curr;
	long mtime = 0;

	gettimeofday(&t_curr, NULL);
    long seconds  = t_curr.tv_sec  - t_global_start.tv_sec;
    long useconds = t_curr.tv_usec - t_global_start.tv_usec;

    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;

	return mtime;
}

void timed_wait_for_task_complition(int timeInMs)
{	

    struct timespec timeToWait;
    struct timeval now;
    int rt;


    pthread_mutex_lock(&taskDoneMutex);
	gettimeofday(&now,NULL);
	timeToWait.tv_sec = now.tv_sec+(unsigned long)(now.tv_usec+1000UL*timeInMs)/1000000UL;
	timeToWait.tv_nsec= (((now.tv_usec+1000UL*timeInMs)*1000UL)%1000000000UL);
    rt = pthread_cond_timedwait(&a_task_is_done, &taskDoneMutex, &timeToWait);
    pthread_mutex_unlock(&taskDoneMutex);
    /*
	if (rt==0)
		printf("A task is done\n");
	else
		printf("error code: %d - [Error Code 110 means it is a timeout]\n",rt);
	*/
}


