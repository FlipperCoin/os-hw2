#include "hw2_test.h"
#include <stdio.h>
#include <iostream>
#include <cassert>
#include <signal.h>
#include <sys/wait.h>
#include <vector>

using namespace std;

int NUMBER_OF_CHILDREN = 10; //whill make 40 processes because each child forks another one
//each child of this process has 3 children with weight = 1

void sig_abort_handler(int sig_num){
	cout << "child " << getpid() << " failed assert" << endl;
	exit(1);
}

string file = "helper.out";
string first_param = "4";
string second_param = "1";

int main() {
	vector<pid_t> children;
	char** arr = new char*[4];
	arr[3] = NULL;
	arr[0] = const_cast<char*>(file.c_str());
	arr[1] = const_cast<char*>(first_param.c_str());
	arr[2] = const_cast<char*>(second_param.c_str());
	signal(SIGABRT, sig_abort_handler);
	int i = 0;
	assert(set_weight(10) == 0);
	int first_child;
	while(i < NUMBER_OF_CHILDREN){
		pid_t child = fork();
		children.push_back(child);
		if(child == 0){
			setpgid(getpid(), children[0]);
			assert(get_weight() == 10);
			file = "./"+file;
			execv(arr[0], arr);
			cerr << "execv failed" << endl;
			exit(1);
		}
		i++;
	}
	sleep(10);
	assert(get_leaf_children_sum () == 1*NUMBER_OF_CHILDREN*3);
	assert(get_heaviest_ancestor () == getpid());
	killpg(children[0], SIGKILL);
	while(waitpid(-1, NULL, 0) > 0);
	cout << "get_leaf_sum passed" << endl;
	assert(set_weight(11) == 0);
	pid_t parent = getpid();
	pid_t child = fork();
	if(child == 0){
		assert(get_weight() == 11);
		assert(set_weight(10) == 0);
		child = fork();
		if(child == 0){
			assert(get_weight() == 10);
			assert(set_weight(3) == 0);
			child = fork();
			if(child == 0){
				assert(get_weight() == 3);
				assert(get_heaviest_ancestor() == parent);
				cout << "get_heaviest_ancestor passed" << endl;
				exit(0);
			}
			waitpid(child, NULL, 0);
			exit(0);
		}
		waitpid(child, NULL, 0);
		exit(0);
	}
	while(waitpid(-1, NULL, 0) > 0);
	assert(set_weight(-1) == -1);
	assert(get_heaviest_ancestor() == getpid());
	assert(get_leaf_children_sum () == -1);
	return 0;
}

