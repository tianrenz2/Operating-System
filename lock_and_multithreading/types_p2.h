#ifndef __TYPES_P2_H
#define __TYPES_P2_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <cstdlib>

#define EMPTY        0
#define WOMENPRESENT 2
#define MENPRESENT   1


class Person
{

	int gender; // 0: male 1: female
	std::string str_gender;
	struct timeval t_create;
	struct timeval t_start;
	struct timeval t_end;
	int time_to_stay_ms;


	unsigned long order;
	unsigned long use_order;

public:
	// int delay;
	Person();

	void set_gender(int data);
	int get_gender(void);

	void set_order(unsigned long data);
	unsigned long get_order(void);

	void set_use_order(unsigned long data);
	unsigned long get_use_order(void);

	void set_time(int data);
	int get_time();
	int ready_to_leave(void);

	void start(void);
	void complete(void);
};


// Class for the restroom
// You may need to add more class member variables and functions
class Restroom {
	

	// You need to define the data structure to
    // save the information of people using the restroom
	// You can probebly use Standard Template Library (STL) vector

public:
	int status;
	Restroom(){
		status = EMPTY;

	}

	// You need to use this function to print the Restroom's status
	void print_status(void);


	// Call by reference
	// This is just an example. You can implement any function you need
	void add_person(Person& p);

};

struct threadParam
{
	Restroom *rr;
	Person *person;
	int id;
};



struct ListNode
{
	Person person;
	ListNode *next;
};

struct RoomListNode
{
	Restroom restroom;
	RoomListNode *next;
};



#endif
