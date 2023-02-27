//lab9 pt 1
//Will Hopkirk


//C++
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <algorithm>
#include <sstream>

//C
#include <stdio.h>
#include <stdlib.h>

using namespace std;

//followed claras psudocode (thx clara you rule)
void do_enumeration(string s, int index, int num){

	if(index == s.size()){
		cout << s << endl;
		return;
	}

	s[index] = '0';

	if((s.size() - index <= num) && (num != 0)){

		s[index] = '1';
	}
	if(s[index] == '1'){

		do_enumeration(s, index + 1, num - 1);
	}else{

		do_enumeration(s, index + 1, num);
	}

	if(s[index] != '1' && num > 0){

		s[index] = '1';
		do_enumeration(s, index + 1, num - 1);
	}

	s[index] = '-';

	return;
}


int main(int argc, char **argv){

	if(argc != 3){
		fprintf(stderr, "usage: enum length nones\n");
		return 1;
	}

	int length, nones;
	string str;

	length = atoi(argv[1]);
	nones = atoi(argv[2]);

	str.resize(length, '-');

	do_enumeration(str, 0, nones);

	return 0;
}

