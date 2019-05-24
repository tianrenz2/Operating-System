#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <pthread.h>
#include "types_p2.h"
#include "p2_threads.h"
#include "utils.h"

void init_queue(int size, int room_num);
void init_rooms(Person *persons, int size);
void removeNode(ListNode *preNode);

int exp_arg = 2;

pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int queue_num;

int gender_in_que[2];
int size_each;

int room_gender = -1;
int man_used;
int woman_used;
int occupied_room;

int man_in_room;
int woman_in_room;

long start_time;

ListNode *head;
RoomListNode *room_head;


struct timeval t_global_start;

int main(int argc, char** argv){
	// This is to set the global start time
	gettimeofday(&t_global_start, NULL);

	start_time = t_global_start.tv_usec;
	pthread_t       tid = 0;
	int             status = 0;
	int             work = 0;

//Checking the number of arguments
	if(argc != exp_arg + 1){
		cout << "Expecting "<< exp_arg <<" argument, but got " << argc - 1 <<endl;
		return 0;
	}

	int size = atoi(argv[1]);
	int room_num = atoi(argv[2]);
	//Define a empty queue of people
	Person queue[size * 2];

	queue_num = size * 2;

	man_used = woman_used = size;

	head = new ListNode; 
	room_head = new RoomListNode;
	//Initialize each element in the queue
	init_queue(size, room_num);

	ListNode *pt = head->next;
	while(pt != NULL){
		cout << pt->person.get_gender()<<", ";
		pt = pt->next;
	}



	cout<<"\n"<<endl;
	// Example code for sleep and class usage
	// Person p1;
	// p1.set_order(1);

	// usleep(MSEC(200));
	// p1.start();


	// usleep(MSEC(150));
	// p1.complete();
	///////////////////////////////////////////

	room_gender = -1;
	occupied_room = 0;
	man_in_room = 0;
	woman_in_room = 0;

	pthread_t queue_thread;

	if(pthread_create(&queue_thread, NULL, roomThreadfunc, NULL)) {
		fprintf(stderr, "Error creating thread\n");		
	}

	pthread_t room_thread;

	if(pthread_create(&room_thread, NULL, queueThreadfunc, NULL)) {
		fprintf(stderr, "Error creating thread\n");		
	}

	// pthread_t threads[room_num];
	// for (int i = 0; i < room_num; ++i)
	// {
	// 	Restroom rr;
	// 	threadParam *tp = (threadParam *) malloc(sizeof(threadParam));;

	// 	tp->rr = rr;
	// 	tp->id = i + 1;
	// 	cout << tp->id << endl;
	// 	if(pthread_create(&threads[i], NULL, threadfunc, tp)) {
	// 		fprintf(stderr, "Error creating thread\n");		
	// 	}
	// }
	pthread_join(room_thread, NULL);
	pthread_join(queue_thread, NULL);

	// for (int i = 0; i < room_num; ++i)
	// {
	// 	pthread_join(threads[i], NULL);
	// }
	
	// usleep(MSEC(10));

	// for (int i=0; i<5; i++) {
	// 	printf("Wake up thread after (%d) seconds\n", (5-i));
	// 	usleep(MSEC(1000));
	// }

	// printf("Wake up thread\n");
	// status = pthread_cond_signal(&cond);
	


	/* wait for the second thread to finish */
	// if(pthread_join(tid, NULL)) {
	// 	fprintf(stderr, "Error joining thread\n");	
	// }
	pthread_exit(NULL);
	return 0;


}


void init_queue(int size, int room_num){

	gender_in_que[0] = 0;
	gender_in_que[1] = 0;
	size_each = size;
	man_used = size;
	woman_used = size;

	RoomListNode *cur = room_head;
	for (int i = 0; i < room_num; ++i)
	{
		/* code */
		Restroom rr;
		rr.status = 0;

		cur->next = new RoomListNode;
		cur->restroom = rr;
		cur = cur->next;
		// cout<<i <<" added"<<endl;
	}

}

void removeNode(ListNode *preNode){
	preNode->next = preNode->next->next;
}
