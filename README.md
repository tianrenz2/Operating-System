# This repository includes UCI EECS 111: Operating-System class projects.

## 1. Multi process and multi threading
The first lab, by using multiple processes and threading, we implemented sorting function on some huge datasets(with each 100000+).
Since the size of datasets is too big, I did not upload them. 

In the program, the number of child processes and threads are defined by user inputs. each process will handle at least one file, the number of threads indicates how many threads each process will create.
In one child process, we have 4 threads, and we have 100000 size of dataset, each thread will sort 25000 rows of the data, the process then will merge all the results when all the threads are done.

## 2. Using multithreading to solve the fitting room problem 

The problem is about some people waiting in a line to use some fitting rooms, numbers of men and weomen are equal but, at one time, there is only one gender allowed to use the fitting rooms. The program is supposed to sechedule all the uses for people in the line and make sure all of them successfully use fitting rooms.

How it is implemented?
Well, the method I used is that I create one thread to dynamically generate people waiting in the queue(this is required for this project), I also used another thread to keep checking the queue(which is a linkedlist), once the thread finds out a qualified person, it removes the person from the queue and start a new thread to execute the part for the person staying in the assigned fitting room.
