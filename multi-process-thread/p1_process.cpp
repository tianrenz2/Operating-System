#include "p1_process.h"

int n = 5;
int task_states[] = {0, 0, 0, 0, 0};

string class_names[] = {"os", "architecture", "java", "algorithm", "digital-design"};


void get_statistics(string class_name[], int num_processes, int num_threads) {

	// You need to create multiple processes here (each process
	// handles at least one file.)
	// fork();
	// printf("Hello\n");

	// Each process should use the sort function which you have defined
	// in the p1_threads.cpp for multithread sorting of the data.

	// Your code should be able to get the statistics and generate
	// the required outputs for any number of child processes and threads.

}

void do_job(int id, int num_threads){
	//This function is executed in each process to do its job
    vector<Entry> read_res = read_files(class_names[id]);
    cout<< "File " <<class_names[id] <<" is being processed"<<endl;
    vector<ThreadChunk> chunks_ret;
    for (int i = 0; i < num_threads; ++i)
    {
        chunks_ret.push_back(ThreadChunk());
    }

    create_threads(num_threads, &read_res, &chunks_ret);
    join_results(chunks_ret, read_res, num_threads, class_names[id]);

	return;
}

void create_processes(int num_processes, int num_threads){
	//Instantiate the processes
	pid_t pids[num_processes];
	int status = 0;
	int wpid;
	cout<< num_processes << " processes"<<endl;

    //Assigning jobs to each process
    vector< vector<int> > job_schedules(num_processes, vector<int>(0));
    for (int i = 0; i < n; ++i)
    {
        int index = i % num_processes;
        job_schedules[index].push_back(i);
    }

	for(int i=0;i< num_processes;i++) // loop will run n times (n=5)
    {
		if(fork() == 0) {
            cout<<"process " << i << " has " << job_schedules[i].size() <<" files to do."<<endl;
            for (int j = 0; j < job_schedules[i].size(); ++j)
            {

                cout<<"Process "<<i << " is doing "<<class_names[job_schedules[i][j]]<<endl;

                do_job(job_schedules[i][j], num_threads);

                cout<<"Process "<<i << " finished "<<class_names[job_schedules[i][j]]<<endl;

            }
            // do_job(0, num_threads);

            exit(0);
        }
    }

   // Wait for all processes to finish
    for (int i = 0; i < num_processes; ++i)
    {
        /* code */
        wait(NULL);
    }

    while ((wpid = wait(&status)) > 0);
}

vector<Entry> read_files(string name){
	//This function reads the corresponding input file name
	vector<Entry> res;
	string filename = "./input/" + name + ".csv";
	// cout<<"Reading " << name <<endl;

	ifstream file(filename.c_str());
	if(!file.is_open()){
        cout << "Unable to open the file"<<endl;
        return res;
    }

    string line;
    int i = 0;
    while(getline(file, line)){
    	if( i > 0){
    		Entry entry;
	    	vector<string> list = split(line, ",");
	    	entry.sid = list[0];
	    	// entry.grade = strtod(list[1].c_str(), NULL);
            entry.grade = strtod(list[1].c_str(), NULL );
            entry.grade_s = list[1];
            // cout<<entry.grade<<endl;
            // entry.grade_d = strtod(list[1].c_str(), NULL);
	    	res.push_back(entry);
    	}
    	i++;
    }

    return res;
}

void write_files(WriteFileArgs args, int type, string filename){
	//For type 1: the output is sorted array
	ofstream myfile;
	myfile.open (filename.c_str());
	if(type == 1){
		// WriteFileArgs *args = (WriteFileArgs*)write_args;
		int size = args.size;
		cout<<"Writing file "<< filename <<", size: "<<size<<endl;
        myfile << "rank, student id, grade"<<"\n";
	}

    cout<<"Write succeed !"<<endl;

	myfile.close();
}

vector<string> split(string str, string token){
    //This function is to split a string
    vector<string>result;
    while(str.size()){
        int index = str.find(token);
        if(index!=string::npos){
            result.push_back(str.substr(0,index));
            str = str.substr(index+token.size());
            if(str.size()==0)result.push_back(str);
        }else{
            result.push_back(str);
            str = "";
        }
    }
    return result;
}

void join_results(vector<ThreadChunk> chunk_ptr, vector<Entry> array, int chunk_num, string class_name){
//This function join all the sorted partition together.

    try{
        divide(0, chunk_num - 1, &array, chunk_ptr);
    }catch (const std::bad_alloc&) {
        cout<<"Bad alloc " << class_name <<endl;
    }


//Write the results to output file
    ofstream myfile;
    myfile.open (("./output/"+ class_name +"_sorted.csv").c_str());
    myfile << "Rank, Student ID, Grade"<<"\n";
    for (int i = 0; i < array.size(); ++i)
    {
            // assert(&args.arr[i] != NULL);
        myfile << i + 1 << ","<< array[i].sid << "," << setprecision(3) <<fixed << array[i].grade <<"\n";
    }
    cout<<"Write succeed !"<<endl;
    myfile.close();


//Calculating standard deviation
    double s = 0;
    for (int i = 0; i < chunk_ptr.size(); ++i)
    {
        // cout<<chunk_ptr[i].sum<<endl;
        s += chunk_ptr[i].sum;
    }

    double ave = s / array.size();
    double std = 0;
    for (int i = 0; i < array.size(); ++i)
    {
        std += pow(array[i].grade - ave, 2);
    }

    std = sqrt(std / array.size());

    ofstream statfile;
    myfile.open (("./output/"+ class_name +"_stats.csv").c_str());
    myfile << "Average,Median,Std. Dev" <<endl;
    myfile<< setprecision(3)  << fixed << s / array.size()  << ", " << setprecision(3) << fixed  << array[array.size()/2].grade <<", "<< setprecision(3) << fixed <<std <<endl;
    statfile.close();

    return;

}

void merge(int l, int r, vector<ThreadChunk> thread_chunks, vector<Entry> *output)
{
    // to store the starting point of left
    // and right array
    ThreadChunk tc_l = thread_chunks[l];
    ThreadChunk tc_r = thread_chunks[r];
    ThreadChunk tc_mid = thread_chunks[((l + r) / 2 + 1)];
    // cout<<"Merge " << tc_l.start << " and " << tc_mid.start <<" and "<<tc_r.end<< " data size: "<< (*output).size() << endl;
    int l_in = tc_l.start, r_in = tc_mid.start;

    // to store the size of left and
    // right array
    int l_c = tc_mid.start - tc_l.start;
    int r_c = tc_r.end - tc_mid.start + 1;

    // array to temporarily store left
    // and right array
    vector<Entry> l_arr;
    vector<Entry> r_arr;
    // // cout<<l_c <<","<<r_c<<endl;
    // // storing data in left array
    for (int i = 0; i < l_c; i++){
    	l_arr.push_back((*output)[l_in + i]);
         // cout<<l_arr[i].grade <<endl;
    }

    // // cout<<"\n";

    // // storing data in right array
    for (int i = 0; i < r_c; i++) {
        r_arr.push_back((*output)[r_in + i]);
        // cout<<r_arr[i].grade <<endl;
    }

  	// cout<<"\n";
    // to store the current index of
    // temporary left and right array
    int l_curr = 0, r_curr = 0;

    // to store the current index
    // for output array
    int in = l_in;
    // cout << tc_mid.start << " - " << tc_r.end;

    int total_l = tc_r.end - tc_l.start;

    // two pointer merge for two sorted arrays
    // cout << "size: " << (l_c + r_c) << endl;

    while (l_curr + r_curr < (l_c + r_c)) {
        // int l_p = tc_l.start + l_curr;
        // int r_p = tc_mid.start + r_curr;
        // cout << "Compare:" << l_p << "->"<< data[l_p].grade <<"," << r_p << "->"<<data[r_p].grade<<endl;
        if (r_curr >= r_c || (l_curr < l_c &&
            // strtod(l_arr[l_curr].grade.c_str(), NULL) < strtod(r_arr[r_curr].grade.c_str(), NULL))){
        	l_arr[l_curr].grade > r_arr[r_curr].grade)){
            // cout << "left: " << l_curr<< ", " << data[l_curr].grade <<endl;
            // assert(l_curr < data.size());
        	(*output)[in] = l_arr[l_curr], l_curr++, in++;
    	}else{
    		// cout << "right: " << r_curr << ", " << data[r_curr].grade <<endl;
             // assert(r_curr < data.size());
            (*output)[in] = r_arr[r_curr], r_curr++, in++;

    	}
    	// cout<<(*output)[in - 1].grade <<endl;
    }
}



// Code to drive merge-sort and
// create recursion tree
void divide(int l, int r, vector<Entry> *output, vector<ThreadChunk> thread_chunks)
{
    // cout<<l <<":"<<r<<endl;
    if (l == r) {
        /* base step to initialize the output
           array before performing merge
           operation */
        return;
    }
    // to sort left half
    // cout<<l << " vs " << (l + r) / 2 << " vs " << r << endl;
    divide(l, (l + r) / 2, output, thread_chunks);

    // cout<<"l: "<< (l + r) / 2 + 1 <<", r: "<< r <<endl;
    // to sort right half
    divide((l + r) / 2 + 1, r, output, thread_chunks);

    // merge the left and right half
    merge(l, r, thread_chunks, output);
}
