#ifndef __P2_THREADS_H
#define __P2_THREADS_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <cmath>

#include "types_p2.h"

using namespace std;

void *threadfunc(void *parm);
void *queueThreadfunc(void *param);
void *roomThreadfunc(void *param);
void *roomThread(void *param);


void add_node(Person p);

void man_wants_to_enter(RoomListNode* cur_room);
void woman_wants_to_enter(RoomListNode* cur_room);

void man_leaves();
void woman_leaves();
void displayRoomList(RoomListNode *node);

#endif
