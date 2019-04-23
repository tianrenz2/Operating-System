#include "p1_threads.h"

// This file includes functions that actually perform the
// computation. You can create a thread with these function
// or directly call this function


void create_threads(int num_threads, vector<Entry> *data, vector<ThreadChunk> *ret_chunks){

//Get the size of data assigned to each thread
  int size_per_chunk = data->size() / num_threads;
  for(int i = 0; i < num_threads; i ++){

    //Threadchunk stores the information of a part of an array,
    //such as start index, end index, sum of that part.
    ThreadChunk tc;

    tc.start = i * size_per_chunk;
    //For the last thread, if there are still data unassigned to, take the rest of them
    if(i == num_threads - 1)
      tc.end =  data->size() - 1;
    else
      tc.end = (i + 1) * size_per_chunk - 1;

    tc.sum = 0;
    (*(ret_chunks))[i] = tc;
  }

	pthread_t threads[num_threads];
	for (long int i = 0 ; i < num_threads ; ++i){
      ThreadArgs thread_args;
      thread_args.id = i;
      thread_args.data = data;
      thread_args.tc = &(*ret_chunks)[i];

      //Here we create a thread and do the sorting job, we pass the reference of the array,
      //chunk infomation into the function so they can be modified
      int t = pthread_create(&threads[i], NULL, thread_job, &thread_args);

      // pthread_detach(threads[i]);
      pthread_join(threads[i], NULL);
      if (t != 0){
        cout << "Error in thread creation: " << t << endl;
      }
  }

  return;
}

void* thread_job(void *thread_args){
  ThreadArgs *args = (ThreadArgs*)thread_args;
  // cout << "I'm a new thread, I'm number " << args->id <<endl;
  // vector<Entry> d = args->data;

  //Each thread gets the start,end information of its own part.
  sort((*(args->data)).begin() + args->tc->start, (*args->data).begin() + args->tc->end + 1, compareEntry);

  double s = 0;
  for (int i = (args->tc)->start; i < args->tc->end + 1; ++i)
  {
    s += (*(args->data))[i].grade;
  }
  args->tc->sum = s;
  // cout<<"Thread sum: "<< s <<endl;
  pthread_exit(NULL);
}

bool compareEntry(Entry e1, Entry e2)
{
    // return (strtod(e1.grade.c_str(), NULL) < strtod(e2.grade.c_str(), NULL));
    return (e1.grade > e2.grade);

}
