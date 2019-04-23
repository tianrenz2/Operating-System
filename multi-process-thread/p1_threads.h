#ifndef __P1_THREADS
#define __P1_THREADS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <pthread.h>
#include <algorithm>

#include "p1_process.h"

//The struct of the arguments passed to each thread's sorting function
struct ThreadArgs
{
	int id;
	//Here we pass the original vectory into the function by reference 
	//so that the vector can be modified in the function.
	vector<Entry> *data;
	ThreadChunk *tc;
};

void* thread_job(void *id);
bool compareEntry(Entry e1, Entry e2);

#endif
