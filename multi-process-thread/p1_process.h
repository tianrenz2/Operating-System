#ifndef __P1_PROCESS
#define __P1_PROCESS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <pthread.h>


#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <dirent.h>

#include <sstream>
#include <string>
#include <assert.h>
#include <iomanip>
#include <cmath>
using namespace std;

// extern char* INPUT_DIR = "./input";

struct Entry
{
	string sid;
	double grade;
	string grade_s;
};

struct ThreadChunk
{
	int start;
	int end;
	double sum;
};

struct ChunkRetArgs
{
	ThreadChunk *ret_chunks;
	vector<Entry> ret_arr;
	int chunk_num;
};

struct WriteFileArgs
{
	vector<Entry> arr;
	int size;
};

void create_processes(int num_processes, int num_threads);

void create_threads(int num_threads, vector<Entry> *data, vector<ThreadChunk> *ret_chunks);


void get_statistics(std::string class_name[], int num_processes, int num_threads);
// void create_processes(int num_processes, int num_threads);
vector<Entry> read_files(string name);
void write_files(WriteFileArgs args, int type, string filename);

vector<string> split(string str, string token);
void join_results(vector<ThreadChunk> chunk_ptr, vector<Entry> array, int chunk_num, string class_name);

void merge(int l, int r, vector<ThreadChunk> thread_chunks, vector<Entry> *output);
void divide(int l, int r, vector<Entry> *output, vector<ThreadChunk> thread_chunks); 

void do_job(int id, int num_threads);

#endif
