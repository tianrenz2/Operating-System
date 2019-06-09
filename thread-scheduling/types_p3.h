#ifndef __TYPES_P3_H
#define __TYPES_P3_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>

 

class ThreadCtrlBlk
{
private:



public:
	int id;
	long period;
	long task_time;

	long start;
	long deadline;
};










#endif
