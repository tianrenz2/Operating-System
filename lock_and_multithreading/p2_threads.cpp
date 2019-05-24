#include "p2_threads.h"
#include "utils.h"

extern pthread_cond_t  cond;
extern pthread_mutex_t mutex;

extern int gender_in_que[2];
extern int size_each;

extern int queue_num;
extern int room_gender;

extern int man_used;
extern int woman_used;

extern int man_in_room;
extern int woman_in_room;

extern int occupied_room;

extern long start_time;
extern struct timeval t_global_start;


extern ListNode *head;
extern RoomListNode *room_head;

extern void removeNode(ListNode *preNode);

void displayList(ListNode *node);

char* gender_names[] = {"man", "woman"};

char* gender_states[] = {"MenPresent", "WomenPresent"};

void *roomThreadfunc(void *param){
	RoomListNode *cur_room = room_head;

	int status;
	while(1){
		if(head->next != NULL){
			ListNode *cur = head;

			if(room_gender != -1){
				while(cur->next != NULL && cur->next->person.get_gender() != room_gender){
					cur = cur->next;
				}
			}
			if(cur->next != NULL){
				// printf("Found a dude\n");

				// displayRoomList(room_head);
				while(cur_room != NULL && cur_room->restroom.status != 0){
					if(cur_room->next != NULL)
						cur_room = cur_room->next;
					else
						cur_room = room_head->next;
				}


				Person p = cur->next->person;
				int gender = p.get_gender();
				
				// printf("A good person: %d\n", cur_room->restroom.status);

				// displayRoomList(room_head);
				if(room_gender != -1 && room_gender != gender)
					continue;

				removeNode(cur);


				if(gender == 0)
					man_wants_to_enter(cur_room);
				else
					woman_wants_to_enter(cur_room);
				
				gettimeofday(&t_global_start, NULL);
				int cur_time = (t_global_start.tv_usec - start_time) / 1000;
				
				int delay = rand()%7 + 3;
				p.set_time(delay);


				int woman_num = woman_in_room;
				int man_num = man_in_room;
				printf("[%d ms][Queue], Send (%s) into the fitting room (Stay %d ms), Status: Total: %d (Men: %d, Women: %d)\n", 
					cur_time, gender_names[gender], p.get_time(), occupied_room, man_num, woman_num);

				printf("[%d ms][fitting room] (%s) goes into the fitting room, State is (%s): Total: %d (Men: %d, Women: %d)\n", 
					cur_time, gender_names[gender], gender_states[gender], occupied_room, man_num, woman_num);

				// printf("room status changed %d\n", cur_room->restroom.status);

				pthread_t room_thread;
				threadParam *tp = (threadParam *) malloc(sizeof(threadParam));;
				tp->rr = &(cur_room->restroom);
				tp->person = &p;
				if(pthread_create(&room_thread, NULL, roomThread, tp)) {
					fprintf(stderr, "Error creating thread\n");		
				}
				

			}

		}
		if(man_used <= 0 && woman_used <= 0){
			printf("All people used\n");
			break;
		}
	}
}

void *roomThread(void *param)
{
	// threadParam *thread_args = (threadParam *) malloc(sizeof(threadParam));
	threadParam *p = (threadParam*)param;
	int gender = p->person->get_gender();
	int wait_time = p->person->get_time();

	usleep(MSEC(wait_time));

	pthread_mutex_lock(&mutex);
	p->rr->status = 0;
	pthread_mutex_unlock(&mutex);
	if(gender == 0)
		man_leaves();
	else
		woman_leaves();

	// gettimeofday(&t_global_start, NULL);
	// cur_time = (t_global_start.tv_usec - start_time) / 1000;

	// printf("[%d ms][fitting rooms], (%s) has left the fitting room, Status: Total: %d (Men: %d, Women: %d)\n",
	 // cur_time, gender_names[gender], man_used + woman_used,man_used, woman_used);

	// printf("Set status %d\n", p->rr->status);
	// displayRoomList(room_head);
}

void *queueThreadfunc(void *param){
	ListNode *pointer = head;
	int status;
	srand(time(0));
	while(gender_in_que[0] < size_each || gender_in_que[1] < size_each){
		
		int gender = rand() % 2;
		Person p;
		if(gender_in_que[gender] >= size_each)
			gender = (gender == 0) ? 1 : 0;

		p.set_gender(gender);

		status = pthread_mutex_lock(&mutex);
		
		add_node(p);

		gender_in_que[gender]++;
		status = pthread_mutex_unlock(&mutex);

		gettimeofday(&t_global_start, NULL);
		int cur_time = (t_global_start.tv_usec - start_time) / 1000;
		printf("[%d ms][Input] A person (%s) goes into the queue\n", cur_time, gender_names[gender]);
		// displayList(head);
		int wait_time = rand() % 3 + 1;
		// for (int i = 0; i < wait_time; ++i)
		// {
		// 	/* code */
		// 	usleep(MSEC(100));
		// }
		usleep(MSEC(wait_time));
	}
}


void displayList(ListNode *node){
	if(node == NULL)
		return;
	ListNode *pt = node->next;
	cout<<"Display:"<<endl;
	int i = 0;
	while(pt != NULL){
		cout<< pt->person.get_gender() <<", ";
		pt = pt->next;

	}
	cout<<"\n"<<endl;
	return;
}

void displayRoomList(RoomListNode *node){
	if(node == NULL)
		return;
	RoomListNode *pt = node->next;
	cout<<"Display:"<<endl;
	int i = 0;
	while(pt != NULL){
		cout<< pt->restroom.status <<", ";
		pt = pt->next;

	}
	cout<<"\n"<<endl;
	return;
}


void man_wants_to_enter(RoomListNode *cur_room){
	pthread_mutex_lock(&mutex);
	occupied_room ++;
	if(room_gender == -1){
		room_gender = 0;
	}
	man_in_room++;
	gettimeofday(&t_global_start, NULL);
	int cur_time = (t_global_start.tv_usec - start_time) / 1000;
	cur_room->restroom.status = 1;
	pthread_mutex_unlock(&mutex);

	return;
}

void woman_wants_to_enter(RoomListNode *cur_room){
	pthread_mutex_lock(&mutex);
	occupied_room ++;
	if(room_gender == -1){
		room_gender = 1;
	}
	woman_in_room++;
	cur_room->restroom.status = 1;
	pthread_mutex_unlock(&mutex);
	return;
}

void man_leaves(){

	printf("wome number %d\n", woman_in_room);

	pthread_mutex_lock(&mutex);
	gettimeofday(&t_global_start, NULL);
	int cur_time = (t_global_start.tv_usec - start_time) / 1000;

	// printf("man number %d\n", man_in_room);
	occupied_room--;
	man_in_room--;
	char* state;
	man_used--;
	pthread_mutex_unlock(&mutex);
	if(occupied_room == 0){
		room_gender = -1;
		state = "empty";
		printf("[%d ms][fitting rooms], (Man) has left the fitting room, Status is changed, status is %s Total: %d (Men: %d, Women: %d)\n", 
			cur_time, state, occupied_room, 0, 0);
	}else{
		// int temp = occupied_room;
		printf("[%d ms][fitting rooms], (Man) has left the fitting room, Status: %s, Total: %d (Men: %d, Women: 0)\n", 
			cur_time, gender_states[room_gender], occupied_room, occupied_room);
	}

	

	return;
}

void woman_leaves(){
	// printf("wome number %d\n", woman_in_room);


	pthread_mutex_lock(&mutex);
	int temp = occupied_room - 1;
	occupied_room--;
	woman_in_room--;

	int women_num = woman_in_room - 1;
	int man_num = man_in_room;
	char* state;
	gettimeofday(&t_global_start, NULL);
	int cur_time = (t_global_start.tv_usec - start_time) / 1000;
	woman_used--;
	pthread_mutex_unlock(&mutex);
	if(occupied_room == 0){
		room_gender = -1;
		state = "empty";
		printf("[%d ms][fitting rooms], (Woman) has left the fitting room, Status is changed, status is %s, Total: %d (Men: %d, Women: %d)\n", 
			cur_time, state, occupied_room, 0, 0);
	}else{
		cout <<"["<< cur_time <<"ms][fitting rooms], (Woman) has left the fitting room, Status:"<< gender_states[room_gender]<<", Total: "<< occupied_room <<" (Men: 0, Women:"<< occupied_room <<")"<<endl;
	}





	return;
}

void add_node(Person p){

	ListNode *cur = head;

	while(cur->next != NULL){
		cur = cur->next;
	}

	cur->next = new ListNode;
	cur->next-> person = p;
}
