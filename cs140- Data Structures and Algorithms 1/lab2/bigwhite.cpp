//lab2 part 2

#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main(int argc, char** argv){

	//vars for constrctuing the white
	int r;
	int c;

	//string stream for reading in the args from c-style strings to ints
	istringstream sin;

	//error check if theres enough args
	if(argc != 3){

		fprintf(stderr, "usage: <bigwhite rows cols>\n");
		return -1;
	}

	//change args from cstrings to ints
	sin.str(argv[1]);
	sin >> r;
	sin.clear();
	sin.str(argv[2]);
	sin >> c;

	// check that args are over 0
	if(r <= 0 || c <= 0){

		fprintf(stderr, "usage: <bigwhite rows cols>\n");
		return -2;
	}

	//print the big white
	printf("P2\n %d %d\n 255\n", c, r);

	return 0;
}

