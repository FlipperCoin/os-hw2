#include <iostream>
#include "unistd.h"
#include "../hw2_test.h"
#include <sys/wait.h>
#include <cassert>
using namespace std;

void sig_abort_handler(int sig_num){
	cout << "child " << getpid() << " failed assert" << endl;
	exit(1);
}

/** argv[1] is parent weight, argv[2] is child weight*/
int main(int argc, char** argv){
	if(argc != 3){
		cout << "try again" << endl;
		exit(0);
	}
	assert( set_weight(stoi(string(argv[1]))) == 0);
	pid_t child = fork();
	if(child == 0){
		assert(get_weight() == stoi(string(argv[1])));
		assert( set_weight(stoi(string(argv[2]))) == 0 );
		exit(0);
	}
	child = fork();
	if(child == 0){
		assert( set_weight(stoi(string(argv[2]))) == 0 );
		exit(0);
	}
	child = fork();
	if(child == 0){
		assert( set_weight(stoi(string(argv[2]))) == 0 );
		exit(0);
	}
	while(1);
	return 0;
}
