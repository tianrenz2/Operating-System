# This repository includes UCI EECS 111: Operating-System class projects.

## 1. Multi process and multi threading
The first lab, by using multiple processes and threading, we implemented sorting function on some huge datasets(with each 100000+).
Since the size of datasets is too big, I did not upload them. 

In the program, the number of child processes and threads are defined by user inputs. each process will handle at least one file, the number of threads indicates how many threads each process will create.
In one child process, we have 4 threads, and we have 100000 size of dataset, each thread will sort 25000 rows of the data, the process then will merge all the results when all the threads are done.

